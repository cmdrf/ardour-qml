#include "Location.h"
#include "QtBridgeUi.h"

#include <ardour/location.h>

Location::Location(QObject* parent, std::shared_ptr<ARDOUR::Location> location) :
	StatefulDestructible{parent, location},
	m_previousFlags{location->flags()}
{
    QtBridgeUi& b = QtBridgeUi::instance();

    // Connect ARDOUR signals directly to our Qt signals where possible
    b.connect(location->NameChanged, this, &Location::nameChanged);
    b.connect(location->StartChanged, this, &Location::startChanged);
    b.connect(location->EndChanged, this, &Location::endChanged);
    b.connect(location->FlagsChanged, this, &Location::onFlagsChanged);
    b.connect(location->LockChanged, this, &Location::lockedChanged);
    b.connect(location->CueChanged, this, &Location::cueIdChanged);
    b.connect(location->TimeDomainChanged, this, &Location::startChanged);
    b.connect(location->TimeDomainChanged, this, &Location::endChanged);
    b.connect(location->Changed, this, &Location::startChanged);
    b.connect(location->Changed, this, &Location::endChanged);
}

void Location::onFlagsChanged()
{
	// Emit only the signals for flags that actually changed
	const ARDOUR::Location::Flags oldf = m_previousFlags;

	if(static_cast<bool>(oldf & ARDOUR::Location::IsMark) != location()->is_mark())
		Q_EMIT markChanged();
	if(static_cast<bool>(oldf & ARDOUR::Location::IsAutoPunch) != location()->is_auto_punch())
		Q_EMIT autoPunchChanged();
	if(static_cast<bool>(oldf & ARDOUR::Location::IsAutoLoop) != location()->is_auto_loop())
		Q_EMIT autoLoopChanged();
	if(static_cast<bool>(oldf & ARDOUR::Location::IsHidden) != location()->is_hidden())
		Q_EMIT hiddenChanged();
	if(static_cast<bool>(oldf & ARDOUR::Location::IsCDMarker) != location()->is_cd_marker())
		Q_EMIT cdMarkerChanged();
	if(static_cast<bool>(oldf & ARDOUR::Location::IsRangeMarker) != location()->is_range_marker())
		Q_EMIT rangeMarkerChanged();
	if(static_cast<bool>(oldf & ARDOUR::Location::IsSkip) != location()->is_skip())
		Q_EMIT skipChanged();

	bool oldSkipping = (oldf & ARDOUR::Location::IsSkip) && (oldf & ARDOUR::Location::IsSkipping);
	if(oldSkipping != location()->is_skipping())
		Q_EMIT skippingChanged();

	if(static_cast<bool>(oldf & ARDOUR::Location::IsClockOrigin) != location()->is_clock_origin())
		Q_EMIT clockOriginChanged();
	if(static_cast<bool>(oldf & ARDOUR::Location::IsSection) != location()->is_section())
		Q_EMIT sectionChanged();

	m_previousFlags = location()->flags();
}
