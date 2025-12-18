#include "Route.h"
#include "Track.h"
#include "QtBridgeUi.h"

#include <ardour/session.h>

Route::Route(QObject *parent, std::shared_ptr<ARDOUR::Route> route) :
	Stripable{parent, route}
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(route->active_changed, this, &Route::activeChanged);

	b.connect(route->mute_control()->Changed, this, &Route::mutedChanged);
	b.connect(route->mute_control()->Changed, this, &Route::mutedByOthersSoloingChanged);
	b.connect(route->session().SoloChanged, this, &Route::mutedByOthersSoloingChanged);
	b.connect(route->solo_control()->Changed, this, &Route::soloedChanged);
	b.connect(route->solo_safe_control()->Changed, this, &Route::isSafeChanged);
	b.connect(route->solo_isolate_control()->Changed, this, &Route::soloIsolatedChanged);
	b.connect(route->track_number_changed, this, &Route::trackNumberChanged);

//	route->gui_changed.connect (route_connections, invalidator (*this), std::bind (&RouteUI::handle_gui_changes, this, _1), gui_context());

	if (isTrack())
	{
//		track()->FreezeChange.connect (*this, invalidator (*this), std::bind (&RouteUI::map_frozen, this), gui_context());
	}
}

Route* Route::create(QObject* parent, std::shared_ptr<ARDOUR::Route> route)
{
	if(auto t = std::dynamic_pointer_cast<ARDOUR::Track>(route))
		return new Track(parent, t);
	return new Route(parent, route);
}

QAbstractItemModel* Route::processors()
{
	if(!m_processors)
		m_processors = new RouteProcessorsModel(this, route());
	return m_processors;
}

QAbstractItemModel* Route::plugins()
{
	if(!m_processors)
		m_processors = new RouteProcessorsModel(this, route());
	if(!m_plugins)
	{
		m_plugins = new QSortFilterProxyModel(this);
		m_plugins->setSourceModel(m_processors);
		m_plugins->setFilterRole(RouteProcessorsModel::TypeRole);
		m_plugins->setFilterFixedString("plugin");

	}
	return m_plugins;
}

QAbstractItemModel* Route::sends()
{
	if(!m_processors)
		m_processors = new RouteProcessorsModel(this, route());
	if(!m_sends)
	{
		m_sends = new QSortFilterProxyModel(this);
		m_sends->setSourceModel(m_processors);
		m_sends->setFilterRole(RouteProcessorsModel::TypeRole);
		m_sends->setFilterFixedString("send");

	}
	return m_sends;
}


