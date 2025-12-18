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
	return lazyCreate<Controllable>(m_soloControl, &ARDOUR::Stripable::solo_control);
}

Controllable* Stripable::muteControl()
{
	return lazyCreate<Controllable>(m_muteControl, &ARDOUR::Stripable::mute_control);
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
