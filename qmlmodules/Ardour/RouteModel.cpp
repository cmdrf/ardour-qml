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
	if(!checkIndex(index, QAbstractItemModel::CheckIndexOption::IndexIsValid | QAbstractItemModel::CheckIndexOption::ParentIsInvalid))
		return QVariant();

	if(role == RouteRole)
		return QVariant::fromValue(m_routes.at(index.row()));
	else if(role == TrackNumberRole)
		return m_routes.at(index.row())->trackNumber();
	return QVariant();
}

QHash<int, QByteArray> RouteModel::roleNames() const
{
	static const QHash<int, QByteArray> roles{
		{RouteRole, "route"},
		{TrackNumberRole, "trackNumber"}
	};
	return roles;
}

void RouteModel::addRoutes(ARDOUR::RouteList& routes)
{
	beginInsertRows(QModelIndex(), m_routes.size(), m_routes.size() + routes.size());
	for(auto& route: routes)
	{
		Route* r = new Route(this, route);
		connect(r, &QObject::destroyed, this, &RouteModel::routeDestroyed);
		connect(r, &Route::trackNumberChanged, this, &RouteModel::updateTrackNumber);
		m_routes.append(r);
	}
	endInsertRows();
}

void RouteModel::routeDestroyed(QObject* obj)
{
	const qsizetype i = m_routes.indexOf(obj);
	if(i < 0)
		return;
	beginRemoveRows(QModelIndex(), i, i);
	m_routes.removeAt(i);
	endRemoveRows();
}

void RouteModel::updateTrackNumber()
{
//	Route* r = qobject_cast<Route*>(QObject::sender());
	const qsizetype i = m_routes.indexOf(QObject::sender());
	if(i < 0)
		return;
	auto index = createIndex(i, 0);
	Q_EMIT dataChanged(index, index, QList<int>({TrackNumberRole}));
}
