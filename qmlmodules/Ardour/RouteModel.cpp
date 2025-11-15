#include "RouteModel.h"
#include "Route.h"


RouteModel::RouteModel(QObject *parent)
	: QAbstractListModel{parent}
{

}

int RouteModel::rowCount(const QModelIndex& parent) const
{
	return m_routes.size();
}

QVariant RouteModel::data(const QModelIndex& index, int role) const
{

}

void RouteModel::addRoutes(ARDOUR::RouteList& routes)
{
	beginInsertRows(QModelIndex(), m_routes.size(), m_routes.size() + routes.size());
	for(auto& route: routes)
	{
		Route* r = new Route(this, route);
		connect(r, &QObject::destroyed, this, &RouteModel::routeDestroyed);
		m_routes.append(r);
	}
	endInsertRows();
}

void RouteModel::routeDestroyed(QObject* obj)
{
	qsizetype i = m_routes.indexOf(obj);
	beginRemoveRows(QModelIndex(), i, i);
	m_routes.removeAt(i);
	endRemoveRows();
}
