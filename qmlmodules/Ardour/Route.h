#ifndef ROUTE_H
#define ROUTE_H

#include "StatefulDestructible.h"
#include "Controllable.h"

#include <ardour/solo_isolate_control.h>
#include <ardour/track.h>
#include <ardour/route.h>

#include <QObject>
#include <QtQmlIntegration>

class Route : public StatefulDestructible
{
	Q_OBJECT
	QML_ANONYMOUS
	Q_DISABLE_COPY(Route)

	Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged FINAL)
	Q_PROPERTY(bool muted READ muted NOTIFY mutedChanged FINAL)
	Q_PROPERTY(bool soloed READ soloed NOTIFY soloedChanged FINAL)
	Q_PROPERTY(bool soloIsolated READ soloIsolated NOTIFY soloIsolatedChanged FINAL)
	Q_PROPERTY(bool isSafe READ isSafe NOTIFY isSafeChanged FINAL)
	Q_PROPERTY(bool canSolo READ canSolo FINAL)
	Q_PROPERTY(bool canMonitor READ canMonitor FINAL)

	/** Numbers >0 are tracks, <0 are busses. 0 is reserved. */
	Q_PROPERTY(qint64 trackNumber READ trackNumber WRITE setTrackNumber NOTIFY trackNumberChanged FINAL)

	Q_PROPERTY(Controllable* muteControl READ muteControl CONSTANT)

public:
	explicit Route(QObject* parent, std::shared_ptr<ARDOUR::Route> route);

	std::shared_ptr<ARDOUR::Route> route() {return std::dynamic_pointer_cast<ARDOUR::Route>(m_stateful);}
	const std::shared_ptr<ARDOUR::Route> route() const {return std::dynamic_pointer_cast<ARDOUR::Route>(m_stateful);}

	bool isTrack () const {return route()->is_track();}

	bool active() const {return route()->active();}
	bool muted() const {return route()->muted();}
	bool soloed() const {return route()->soloed();}
	bool soloIsolated() const {return route()->solo_isolate_control()->solo_isolated();}
	bool isSafe() const {return route()->is_safe();}
	bool canSolo() const {return route()->can_solo();}
	bool canMonitor() const {return route()->can_monitor();}
	qint64 trackNumber() const {return route()->track_number();}

	void setActive(bool active) {route()->set_active(active, nullptr);}
	void setTrackNumber(qint64 trackNumber) {route()->set_track_number(trackNumber);}

	Controllable* muteControl() {return m_muteControl;}

Q_SIGNALS:
	void activeChanged();
	void mutedChanged();
	void soloedChanged();
	void soloIsolatedChanged();
	void isSafeChanged();
	void trackNumberChanged();

private:
	QPointer<Controllable> m_muteControl;
};

#endif // ROUTE_H
