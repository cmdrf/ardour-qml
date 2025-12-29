#include "Processor.h"
#include "TriggerBox.h"

Processor::Processor(QObject *parent, std::shared_ptr<ARDOUR::Processor> processor)
	: SessionObject{parent, processor}
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(processor->ActiveChanged, this, &Processor::activeChanged);
	b.connect(processor->ActiveChanged, this, &Processor::enabledChanged); // Need to disconnect in subclasses?
	b.connect(processor->BypassableChanged, this, &Processor::bypassableChanged);
}

Processor* Processor::create(QObject* parent, std::shared_ptr<ARDOUR::Processor> processor)
{
	if(auto t = std::dynamic_pointer_cast<ARDOUR::TriggerBox>(processor))
		return new TriggerBox(parent, t);
	// TODO: PluginInsert, Send
	return new Processor(parent, processor);
}

bool Processor::enabled() const
{
	return processor()->enabled();
}

void Processor::setEnabled(bool newEnabled)
{
	processor()->enable(newEnabled);
}

bool Processor::active() const
{
	return processor()->active();
}

void Processor::setActive(bool newActive)
{
	if(newActive)
		processor()->activate();
	else
		processor()->deactivate();
}

bool Processor::bypassable() const
{
	return processor()->bypassable();
}
