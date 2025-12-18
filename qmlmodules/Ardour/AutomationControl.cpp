#include "AutomationControl.h"


AutomationControl::AutomationControl(QObject* parent, std::shared_ptr<ARDOUR::AutomationControl> automationControl)
	: Controllable{parent, automationControl}
{

}
