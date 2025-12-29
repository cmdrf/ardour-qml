#include "TriggerBox.h"


TriggerBox::TriggerBox(QObject* parent, std::shared_ptr<ARDOUR::TriggerBox> triggerBox)
	: Processor{parent, triggerBox}
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(triggerBox->RecEnableChanged, this, &TriggerBox::recordEnabledChanged);
	b.connect(triggerBox->ArmedChanged, this, &TriggerBox::armedChanged);
	b.connect(triggerBox->EmptyStatusChanged, this, &TriggerBox::emptyChanged);
}






