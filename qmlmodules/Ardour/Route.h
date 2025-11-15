#ifndef ROUTE_H
#define ROUTE_H

#include <ardour/solo_isolate_control.h>
#include <ardour/track.h>
#include <ardour/route.h>

#include <QObject>
#include <QtQmlIntegration>

class Route : public QObject
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

public:
	explicit Route(QObject* parent, std::shared_ptr<ARDOUR::Route> route);

	bool isTrack () const {return m_route->is_track();}

	bool active() const {return m_route->active();}
	bool muted() const {return m_route->muted();}
	bool soloed() const {return m_route->soloed();}
	bool soloIsolated() const {return m_route->solo_isolate_control()->solo_isolated();}
	bool isSafe() const {return m_route->is_safe();}
	bool canSolo() const {return m_route->can_solo();}
	bool canMonitor() const {return m_route->can_monitor();}

	void setActive(bool active) {m_route->set_active(active, nullptr);}

Q_SIGNALS:
	void activeChanged();
	void mutedChanged();
	void soloedChanged();
	void soloIsolatedChanged();
	void isSafeChanged();

private:
	std::shared_ptr<ARDOUR::Route> m_route;
};

#endif // ROUTE_H
