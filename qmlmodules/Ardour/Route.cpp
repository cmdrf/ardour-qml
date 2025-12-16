#include "Route.h"
#include "QtBridgeUi.h"

#include <ardour/session.h>

Route::Route(QObject *parent, std::shared_ptr<ARDOUR::Route> route) :
	Stripable{parent, route},
	m_soloControl(new Controllable(this, route->solo_control())),
	m_muteControl(new Controllable(this, route->mute_control()))
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


