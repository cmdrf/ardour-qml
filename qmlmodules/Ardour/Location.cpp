#include "Location.h"
#include "QtBridgeUi.h"

#include <ardour/location.h>

Location::Location(QObject* parent, std::shared_ptr<ARDOUR::Location> location)
    : StatefulDestructible{parent, location}
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
    // FlagsChanged is a coarse signal that may indicate any of several boolean properties changed.
    Q_EMIT markChanged();
    Q_EMIT autoPunchChanged();
    Q_EMIT autoLoopChanged();
    Q_EMIT hiddenChanged();
    Q_EMIT cdMarkerChanged();
    Q_EMIT rangeMarkerChanged();
    Q_EMIT skipChanged();
    Q_EMIT skippingChanged();
    Q_EMIT clockOriginChanged();
    Q_EMIT sectionChanged();
}
