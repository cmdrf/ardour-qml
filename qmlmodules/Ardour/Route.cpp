#include "Route.h"
#include "QtBridgeUi.h"

Route::Route(QObject *parent, std::shared_ptr<ARDOUR::Route> route) :
	QObject{parent},
	m_route(route)
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(route->DropReferences, this, &QObject::deleteLater);

	b.connect(route->active_changed, this, SIGNAL(activeChanged));

	b.connect(route->mute_control()->Changed, this, &Route::mutedChanged);
	b.connect(route->solo_control()->Changed, this, &Route::soloedChanged);
	b.connect(route->solo_safe_control()->Changed, this, &Route::isSafeChanged);
	b.connect(route->solo_isolate_control()->Changed, this, &Route::soloIsolatedChanged);

//	route->gui_changed.connect (route_connections, invalidator (*this), std::bind (&RouteUI::handle_gui_changes, this, _1), gui_context());

	if (isTrack())
	{
//		track()->FreezeChange.connect (*this, invalidator (*this), std::bind (&RouteUI::map_frozen, this), gui_context());
	}

}


