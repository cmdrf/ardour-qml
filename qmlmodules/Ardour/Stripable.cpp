#include "Stripable.h"

#include <ardour/stripable.h>
#include <ardour/solo_control.h>
#include <ardour/mute_control.h>

Stripable::Stripable(QObject *parent, std::shared_ptr<ARDOUR::Stripable> stripable) :
	SessionObject{parent, stripable},
	m_soloControl(new Controllable(this, stripable->solo_control())),
	m_muteControl(new Controllable(this, stripable->mute_control()))
{

}
