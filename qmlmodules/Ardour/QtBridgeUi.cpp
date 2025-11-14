#include "QtBridgeUi.h"

#include <pbd/abstract_ui.cc>  /* instantiate the template */

#include <QDebug>
#include <QMetaMethod>
#include <QPointer>

QtBridgeUi::QtBridgeUi(QObject *parent) :
	QObject{parent},
	AbstractUI<QtBridgeUiRequest>("QtBridgeUi")

{

}


QtBridgeUi& QtBridgeUi::instance()
{
	static QtBridgeUi inst;
	return inst;
}

void QtBridgeUi::connect(PBD::Signal0<void>& signal, QObject* receiver, const char* method)
{
	QPointer<QObject> receiverPtr(receiver);
	int methodIndex = receiver->metaObject()->indexOfMethod(method);
	if(methodIndex < 0)
	{
		qWarning() << "Method" << method << "not found.";
		return;
	}

	// keep the connection alive in a shared_ptr so the lambda that is stored on `receiver`
	// can call disconnect later without dangling references:
	auto conn = std::make_shared<PBD::ScopedConnection>();

	// Install the signal handler; the ScopedConnection instance will hold the connection handle:
	signal.connect(*conn, MISSING_INVALIDATOR,
				   [receiverPtr, methodIndex](){
						if(receiverPtr)
							receiverPtr->metaObject()->method(methodIndex).invoke(receiverPtr, Qt::QueuedConnection);
				   },
				   this);

	// When the QObject is destroyed, disconnect the PBD connection:
	QObject::connect(receiver, &QObject::destroyed, [conn](){
		conn->disconnect();
	});
}

void QtBridgeUi::thread_init()
{

}

void QtBridgeUi::do_request(QtBridgeUiRequest* req)
{
	if(req->type == CallSlot)
		req->the_slot();
}
