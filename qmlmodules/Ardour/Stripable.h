#ifndef STRIPABLE_H
#define STRIPABLE_H

#include "SessionObject.h"
#include "AutomationControl.h"
#include "PresentationInfo.h"

class Stripable : public SessionObject
{
	Q_OBJECT

	Q_PROPERTY(bool selected READ selected NOTIFY selectedChanged FINAL)

	Q_PROPERTY(AutomationControl* gainControl READ gainControl CONSTANT)
	Q_PROPERTY(AutomationControl* soloControl READ soloControl CONSTANT)
	Q_PROPERTY(AutomationControl* muteControl READ muteControl CONSTANT)
	Q_PROPERTY(AutomationControl* phaseControl READ phaseControl CONSTANT)
	Q_PROPERTY(AutomationControl* trimControl READ trimControl CONSTANT)
	Q_PROPERTY(AutomationControl* monitoringControl READ monitoringControl CONSTANT)
	Q_PROPERTY(PresentationInfo* presentationInfo READ presentationInfo CONSTANT)

public:
	explicit Stripable(QObject* parent, std::shared_ptr<ARDOUR::Stripable> stripable);

	std::shared_ptr<ARDOUR::Stripable> stripable() {return std::dynamic_pointer_cast<ARDOUR::Stripable>(m_stateful);}
	const std::shared_ptr<ARDOUR::Stripable> stripable() const {return std::dynamic_pointer_cast<ARDOUR::Stripable>(m_stateful);}

	bool selected() const;
	PresentationInfo* presentationInfo();

	AutomationControl* gainControl();
	AutomationControl* soloControl();
	AutomationControl* muteControl();
	AutomationControl* phaseControl();
	AutomationControl* trimControl();
	AutomationControl* monitoringControl();

Q_SIGNALS:
	void selectedChanged();

private Q_SLOTS:
	void handlePresentationChange();

private:
	template<class C, typename F>
	C* lazyCreate(QPointer<C>& pointer, F getter);

	bool m_selected;
	QPointer<PresentationInfo> m_presentationInfo;
	QPointer<AutomationControl> m_gainControl;
	QPointer<AutomationControl> m_soloControl;
	QPointer<AutomationControl> m_muteControl;
	QPointer<AutomationControl> m_phaseControl;
	QPointer<AutomationControl> m_trimControl;
	QPointer<AutomationControl> m_monitoringControl;
};

#endif // STRIPABLE_H
