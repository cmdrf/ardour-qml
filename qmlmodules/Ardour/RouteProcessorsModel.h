#ifndef ROUTEPROCESSORSMODEL_H
#define ROUTEPROCESSORSMODEL_H

#include "Processor.h"

#include <ardour/route.h>

#include <QAbstractListModel>

class RouteProcessorsModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum
	{
		ProcessorRole = Qt::UserRole,

		/** Can be "plugin", "send" or empty. */
		TypeRole
	};

	RouteProcessorsModel(QObject* parent, std::shared_ptr<ARDOUR::Route> route);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

	bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

private Q_SLOTS:
	void handleProcessorChanges(ARDOUR::RouteProcessorChange change);
	void removeProcessor(QObject* processor);

private:
	void populate();

	std::shared_ptr<ARDOUR::Route> m_route;
	QVector<QPointer<Processor>> m_processors;
};

#endif // ROUTEPROCESSORSMODEL_H
