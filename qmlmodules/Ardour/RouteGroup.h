#ifndef ROUTEGROUP_H
#define ROUTEGROUP_H

#include "SessionObject.h"

class RouteGroup : public SessionObject
{
	Q_OBJECT

public:
	RouteGroup(QObject* parent, std::shared_ptr<ARDOUR::RouteGroup> routeGroup);

	std::shared_ptr<ARDOUR::RouteGroup> routeGroup() {return std::dynamic_pointer_cast<ARDOUR::RouteGroup>(m_stateful);}
	const std::shared_ptr<ARDOUR::RouteGroup> routeGroup() const {return std::dynamic_pointer_cast<ARDOUR::RouteGroup>(m_stateful);}
};

#endif // ROUTEGROUP_H
