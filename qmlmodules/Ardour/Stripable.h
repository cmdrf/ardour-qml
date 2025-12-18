#ifndef STRIPABLE_H
#define STRIPABLE_H

#include "SessionObject.h"
#include "Controllable.h"

class Stripable : public SessionObject
{
	Q_OBJECT

	Q_PROPERTY(bool selected READ selected NOTIFY selectedChanged FINAL)

	Q_PROPERTY(Controllable* gainControl READ gainControl CONSTANT)
	Q_PROPERTY(Controllable* soloControl READ soloControl CONSTANT)
	Q_PROPERTY(Controllable* muteControl READ muteControl CONSTANT)
	Q_PROPERTY(Controllable* phaseControl READ phaseControl CONSTANT)
	Q_PROPERTY(Controllable* trimControl READ trimControl CONSTANT)
	Q_PROPERTY(Controllable* monitoringControl READ monitoringControl CONSTANT)

public:
	explicit Stripable(QObject* parent, std::shared_ptr<ARDOUR::Stripable> stripable);

	std::shared_ptr<ARDOUR::Stripable> stripable() {return std::dynamic_pointer_cast<ARDOUR::Stripable>(m_stateful);}
	const std::shared_ptr<ARDOUR::Stripable> stripable() const {return std::dynamic_pointer_cast<ARDOUR::Stripable>(m_stateful);}

	bool selected() const;

	Controllable* gainControl();
	Controllable* soloControl();
	Controllable* muteControl();
	Controllable* phaseControl();
	Controllable* trimControl();
	Controllable* monitoringControl();

Q_SIGNALS:
	void selectedChanged();

private Q_SLOTS:
	void handlePresentationChange();

private:
	template<class C, typename F>
	C* lazyCreate(QPointer<C>& pointer, F getter);

	bool m_selected;
	QPointer<Controllable> m_gainControl;
	QPointer<Controllable> m_soloControl;
	QPointer<Controllable> m_muteControl;
	QPointer<Controllable> m_phaseControl;
	QPointer<Controllable> m_trimControl;
	QPointer<Controllable> m_monitoringControl;
};

#endif // STRIPABLE_H
