#ifndef ROUTEGROUP_H
#define ROUTEGROUP_H

#include "SessionObject.h"

class RouteGroup : public SessionObject
{
	Q_OBJECT

public:
	RouteGroup(QObject* parent, std::shared_ptr<ARDOUR::RouteGroup> routeGroup);
};

#endif // ROUTEGROUP_H
