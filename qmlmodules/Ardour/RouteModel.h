#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <ardour/types.h>

#include <QAbstractListModel>

class Route;

class RouteModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum
	{
		RouteRole = Qt::UserRole,
		TrackNumberRole
	};

	explicit RouteModel(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

public Q_SLOTS:
	void addRoutes(ARDOUR::RouteList& routes);

private Q_SLOTS:
	void routeDestroyed(QObject* obj);
	void updateTrackNumber();

private:
	QVector<Route*> m_routes;
};

#endif // ROUTEMODEL_H
