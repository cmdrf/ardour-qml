#include "Stripable.h"

#include <ardour/stripable.h>
#include <ardour/gain_control.h>
#include <ardour/solo_control.h>
#include <ardour/mute_control.h>
#include <ardour/phase_control.h>
#include <ardour/monitor_control.h>

Stripable::Stripable(QObject *parent, std::shared_ptr<ARDOUR::Stripable> stripable) :
	SessionObject{parent, stripable}
{

}

Controllable* Stripable::gainControl()
{
	return lazyCreate(m_gainControl, &ARDOUR::Stripable::gain_control);
}

Controllable* Stripable::soloControl()
{
	return lazyCreate(m_soloControl, &ARDOUR::Stripable::solo_control);
}

Controllable* Stripable::muteControl()
{
	return lazyCreate(m_muteControl, &ARDOUR::Stripable::mute_control);
}

Controllable* Stripable::phaseControl()
{
	return lazyCreate(m_phaseControl, &ARDOUR::Stripable::phase_control);
}

Controllable* Stripable::trimControl()
{
	return lazyCreate(m_trimControl, &ARDOUR::Stripable::trim_control);
}

Controllable* Stripable::monitoringControl()
{
	return lazyCreate(m_monitoringControl, &ARDOUR::Stripable::monitoring_control);
}

template<class C, typename F>
C* Stripable::lazyCreate(QPointer<C>& pointer, F getter)
{
	if(pointer)
		return pointer;
	else
	{
		auto s = stripable();
		if(auto c = std::invoke(getter, s.get()))
		{
			pointer = new C(this, c);
			return pointer;
		}
	}
	return nullptr;
}
