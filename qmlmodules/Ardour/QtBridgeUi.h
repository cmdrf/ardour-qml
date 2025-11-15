#ifndef QTBRIDGEUI_H
#define QTBRIDGEUI_H

#include <QPointer>
#include <QMetaObject>
#include <QObject>

#include <pbd/abstract_ui.h>

#include <functional>
#include <memory>
#include <tuple>
#include <utility>
#include <type_traits>

#include <QObject>

namespace PBD_QTBRIDGE {

// memfn_traits: extract class type and arg types from a member function pointer.
template<typename T>
struct memfn_traits;

// non-const member function
template<typename R, typename C, typename... A>
struct memfn_traits<R (C::*)(A...)>
{
	using class_type = C;
	using return_type = R;
	using args_tuple = std::tuple<A...>;
	static constexpr std::size_t arity = sizeof...(A);
	using arg_types = std::tuple<A...>;
};

// const member function
template<typename R, typename C, typename... A>
struct memfn_traits<R (C::*)(A...) const>
{
	using class_type = C;
	using return_type = R;
	using args_tuple = std::tuple<A...>;
	static constexpr std::size_t arity = sizeof...(A);
	using arg_types = std::tuple<A...>;
};

// helper: call member function pointer with a tuple, using only first N elements
template<typename MemFn, typename ReceiverPtr, typename Tuple, std::size_t... I>
inline void call_member_with_indices(MemFn memfn, ReceiverPtr receiver, Tuple &&t, std::index_sequence<I...>)
{
	// std::invoke will accept a member function pointer and an object pointer as first parameter
	std::invoke(memfn, static_cast<typename memfn_traits<MemFn>::class_type*>(receiver), std::get<I>(std::forward<Tuple>(t))...);
}

// general entry to call a member pointer using first N args of tuple
template<std::size_t N, typename MemFn, typename ReceiverPtr, typename Tuple>
inline void call_member_take_first_n(MemFn memfn, ReceiverPtr receiver, Tuple &&t)
{
	call_member_with_indices<MemFn, ReceiverPtr>(
		memfn, receiver, std::forward<Tuple>(t),
		std::make_index_sequence<N>{}
	);
}
} // namespace PBD_QTBRIDGE

struct QtBridgeUiRequest : public BaseUI::BaseRequestObject
{
  public:
	QtBridgeUiRequest () { }
	~QtBridgeUiRequest() { }
};

class QtBridgeUi : public QObject, public AbstractUI<QtBridgeUiRequest>
{
	Q_OBJECT
	Q_DISABLE_COPY(QtBridgeUi)

public:
	explicit QtBridgeUi(QObject *parent = nullptr);
	~QtBridgeUi() {}; // Required for some reason

	static QtBridgeUi& instance();

	void connect(PBD::Signal<void ()>& signal, QObject *receiver, const char *method);

	/// Primary connect template.
	/** - Signal signature: R_sig(SignalArgs...)
		- Receiver must be pointer to QObject (e.g. MyWidget*).
		- Slot must be a member function pointer: Ret (ReceiverType::*)(SlotArgs...) or const variant.

		@returns std::shared_ptr<PBD::ScopedConnection> so caller can keep it if needed. */
	template<typename R_sig, typename... SignalArgs, typename Receiver, typename SlotMemFn,
			// SFINAE: this template only participates if SlotMemFn is a member function pointer
			typename = typename std::enable_if<std::is_member_function_pointer<SlotMemFn>::value>::type
	>
	std::shared_ptr<PBD::ScopedConnection>
	connect(PBD::Signal<R_sig(SignalArgs...)> &signal, Receiver *receiver, SlotMemFn slot_memfn)
	{
		// Basic sanity checks at compile time:
		static_assert(std::is_base_of<QObject, Receiver>::value,
					  "Receiver must be a QObject-derived type (pointer).");

		using SlotTraits = PBD_QTBRIDGE::memfn_traits<SlotMemFn>;
		using SlotClass = typename SlotTraits::class_type;
		static_assert(std::is_base_of<SlotClass, Receiver>::value || std::is_same<SlotClass, Receiver>::value,
					  "Member function pointer class type must match receiver type (or a base).");

		constexpr std::size_t slot_arity = SlotTraits::arity;
		constexpr std::size_t signal_arity = sizeof...(SignalArgs);
		static_assert(slot_arity <= signal_arity,
					  "Slot expects more arguments than signal provides. The slot may take fewer args than the signal, not more.");

		// store receiver safely
		QPointer<Receiver> receiverPtr(receiver);

		// keep connection handle in shared_ptr so both the lambda and the QObject->destroyed handler can refer to it
		auto conn = std::make_shared<PBD::ScopedConnection>();

		// create the PBD signal handler. It accepts the full SignalArgs...;
		// we capture a copy of the arguments into a tuple and then post a queued functor to the receiver's thread
		signal.connect(*conn, MISSING_INVALIDATOR,
			[receiverPtr, slot_memfn](SignalArgs... args) mutable {
				// if receiver was already deleted we can return early (but we still want to be RT-safe).
				// capture args by value into a tuple (decay-copy)
				auto argsTuple = std::make_tuple(std::forward<SignalArgs>(args)...);

				// build the task that will be executed in receiver's thread
				auto task = [receiverPtr, slot_memfn, argsTuple = std::move(argsTuple)]() mutable {
					Receiver *r = receiverPtr.data();
					if (!r)
						return; // receiver gone
					// call the member pointer using only the first 'slot_arity' elements of argsTuple
					PBD_QTBRIDGE::call_member_take_first_n<slot_arity, SlotMemFn, Receiver*, decltype(argsTuple)>(
						slot_memfn, static_cast<Receiver*>(r), std::move(argsTuple));
				};

				// Post to receiver's thread as queued invocation.
				if (receiverPtr)
					QMetaObject::invokeMethod(receiverPtr, std::move(task), Qt::QueuedConnection);
			},
			this
		);

		// When the QObject is destroyed, disconnect the PBD connection:
		QObject::connect(receiver, &QObject::destroyed, [conn]() {
			conn->disconnect();
		});

		return conn;
	}


protected:
	void thread_init();
	void do_request(QtBridgeUiRequest*);

private:
//	PBD::ScopedConnectionList& connectionList;
};

#endif // QTBRIDGEUI_H
