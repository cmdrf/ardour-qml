#include "RouteGroup.h"

#include <ardour/route_group.h>

RouteGroup::RouteGroup(QObject* parent, std::shared_ptr<ARDOUR::RouteGroup> routeGroup)
	: SessionObject{parent, routeGroup}
{

}
