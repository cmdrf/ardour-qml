#ifndef AUTOMATIONCONTROL_H
#define AUTOMATIONCONTROL_H

#include "Controllable.h"
#include <ardour/automation_control.h>

class AutomationControl : public Controllable
{
	Q_OBJECT
public:
	explicit AutomationControl(QObject* parent, std::shared_ptr<ARDOUR::AutomationControl> automationControl);

	std::shared_ptr<ARDOUR::AutomationControl> automationControl() {return std::dynamic_pointer_cast<ARDOUR::AutomationControl>(m_stateful);}
	const std::shared_ptr<ARDOUR::AutomationControl> automationControl() const {return std::dynamic_pointer_cast<ARDOUR::AutomationControl>(m_stateful);}

};

#endif // AUTOMATIONCONTROL_H
