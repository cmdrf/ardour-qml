#include "Stripable.h"

#include <ardour/stripable.h>
#include <ardour/solo_control.h>
#include <ardour/mute_control.h>

Stripable::Stripable(QObject *parent, std::shared_ptr<ARDOUR::Stripable> stripable) :
	SessionObject{parent, stripable}
{

}

Controllable* Stripable::soloControl()
{
	if(m_soloControl)
		return m_soloControl;
	else
	{
		auto s = stripable();
		if(auto c = s->solo_control())
		{
			m_soloControl = new Controllable(this, c);
			return m_soloControl;
		}
	}
	return nullptr;
}

Controllable* Stripable::muteControl()
{
	if(m_muteControl)
		return m_muteControl;
	else
	{
		auto s = stripable();
		if(auto c = s->mute_control())
		{
			m_muteControl = new Controllable(this, c);
			return m_muteControl;
		}
	}
	return nullptr;
}
