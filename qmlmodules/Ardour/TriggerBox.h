#ifndef TRIGGERBOX_H
#define TRIGGERBOX_H

#include "Processor.h"

#include <ardour/triggerbox.h>

class TriggerBox : public Processor
{
	Q_OBJECT

	Q_PROPERTY(bool recordEnabled READ recordEnabled WRITE setRecordEnabled NOTIFY recordEnabledChanged FINAL)
	Q_PROPERTY(bool armed READ armed NOTIFY armedChanged FINAL)
	Q_PROPERTY(bool empty READ empty NOTIFY emptyChanged FINAL)

public:
	explicit TriggerBox(QObject* parent, std::shared_ptr<ARDOUR::TriggerBox> triggerBox);

	std::shared_ptr<ARDOUR::TriggerBox> triggerBox() {return std::dynamic_pointer_cast<ARDOUR::TriggerBox>(m_stateful);}
	const std::shared_ptr<ARDOUR::TriggerBox> triggerBox() const {return std::dynamic_pointer_cast<ARDOUR::TriggerBox>(m_stateful);}

	bool recordEnabled() const {return triggerBox()->record_enabled() == ARDOUR::Enabled;}
	void setRecordEnabled(bool newRecordEnabled) {triggerBox()->set_record_enabled(newRecordEnabled);}
	bool armed() const {return triggerBox()->armed();}
	bool empty() const {return triggerBox()->empty();}

public Q_SLOTS:
	void disarm() {triggerBox()->disarm();}

Q_SIGNALS:
	void recordEnabledChanged();
	void armedChanged();
	void emptyChanged();

private:
};

#endif // TRIGGERBOX_H
