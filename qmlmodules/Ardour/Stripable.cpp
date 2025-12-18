#include "Stripable.h"

#include <ardour/stripable.h>
#include <ardour/gain_control.h>
#include <ardour/solo_control.h>
#include <ardour/mute_control.h>
#include <ardour/phase_control.h>
#include <ardour/monitor_control.h>

Stripable::Stripable(QObject *parent, std::shared_ptr<ARDOUR::Stripable> stripable) :
	SessionObject{parent, stripable},
	m_selected(stripable->is_selected())
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(ARDOUR::PresentationInfo::Change, this, &Stripable::handlePresentationChange);

}

bool Stripable::selected() const
{
	return m_selected;
}

PresentationInfo* Stripable::presentationInfo()
{
	return lazyCreate(m_presentationInfo, &ARDOUR::Stripable::presentation_info_ptr);
}


AutomationControl* Stripable::gainControl()
{
	return lazyCreate(m_gainControl, &ARDOUR::Stripable::gain_control);
}

AutomationControl* Stripable::soloControl()
{
	return lazyCreate(m_soloControl, &ARDOUR::Stripable::solo_control);
}

AutomationControl* Stripable::muteControl()
{
	return lazyCreate(m_muteControl, &ARDOUR::Stripable::mute_control);
}

AutomationControl* Stripable::phaseControl()
{
	return lazyCreate(m_phaseControl, &ARDOUR::Stripable::phase_control);
}

AutomationControl* Stripable::trimControl()
{
	return lazyCreate(m_trimControl, &ARDOUR::Stripable::trim_control);
}

AutomationControl* Stripable::monitoringControl()
{
	return lazyCreate(m_monitoringControl, &ARDOUR::Stripable::monitoring_control);
}

void Stripable::handlePresentationChange()
{
	bool newSelected = stripable()->is_selected();
	if(newSelected != m_selected)
	{
		m_selected = newSelected;
		Q_EMIT selectedChanged();
	}
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
