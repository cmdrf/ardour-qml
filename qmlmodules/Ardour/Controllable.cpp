#include "Controllable.h"
#include "QtBridgeUi.h"


Controllable::Controllable(QObject *parent, std::shared_ptr<PBD::Controllable> controllable)
	: StatefulDestructible{parent, controllable}
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(controllable->Changed, this, &Controllable::valueChanged);
	b.connect(controllable->TouchChanged, this, &Controllable::touchingChanged);
}

double Controllable::value() const
{
	return controllable()->get_value();
}

void Controllable::setValue(double newValue)
{
	controllable()->set_value(newValue, PBD::Controllable::NoGroup);
}

bool Controllable::touching() const
{
	return controllable()->touching();
}
