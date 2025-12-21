#include "RouteProcessorsModel.h"

#include <ardour/amp.h>
#include <ardour/send.h>
#include <ardour/plugin_insert.h>
#include <ardour/internal_send.h>


RouteProcessorsModel::RouteProcessorsModel(QObject* parent, std::shared_ptr<ARDOUR::Route> route) :
	QAbstractListModel(parent),
	m_route(route)
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(route->processors_changed, this, &RouteProcessorsModel::handleProcessorChanges);
	populate();
}

int RouteProcessorsModel::rowCount(const QModelIndex& parent) const
{
	return m_processors.count();
}

QVariant RouteProcessorsModel::data(const QModelIndex& index, int role) const
{
	if(index.row() >= m_processors.size() || index.column() != 0)
		return QVariant();

	if(role == ProcessorRole)
	{
		return QVariant::fromValue(m_processors.at(index.row()).get());
	}
	else if(role == TypeRole)
	{
		auto p = m_processors.at(index.row())->processor();
		if(dynamic_cast<ARDOUR::PluginInsert*>(p.get()))
			return "plugin";
		else if(auto send = std::dynamic_pointer_cast<ARDOUR::Send>(p))
		{
			// Mimic Route::nth_send:
			if(send == m_route->monitor_send())
				return ""; // Monitor send is not an accessible send
			else
				return "send";
		}
		else if(dynamic_cast<ARDOUR::Amp*>(p.get()))
			return "amp";
		else if(dynamic_cast<ARDOUR::UnknownProcessor*>(p.get()))
			return "unknown";
		else
			return "";
	}
	return QVariant();
}

QHash<int, QByteArray> RouteProcessorsModel::roleNames() const
{
	static const QHash<int, QByteArray> roles{
		{ProcessorRole, "processor"},
		{TypeRole, "type"}
	};
	return roles;
}

bool RouteProcessorsModel::moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild)
{
	// Only top level:
	if(sourceParent.isValid() || destinationParent.isValid())
		return false;

	ARDOUR::Route::ProcessorList newOrder;

	// Fill in old processors up to destinationChild:
	for(int i = 0; i < destinationChild; ++i)
		newOrder.push_back(m_processors.at(i)->processor());

	// Fill in moved processors:
	for(int i = 0; i < count; ++i)
		newOrder.push_back(m_processors.at(sourceRow + i)->processor());

	// Fill in remaining processors:
	for(int i = destinationChild; i < m_processors.size(); ++i)
	{
		if(i < sourceRow || i >= sourceRow + count)
			newOrder.push_back(m_processors.at(i)->processor());
	}

	Q_ASSERT(newOrder.size() == m_processors.size());

	return (m_route->reorder_processors(newOrder) == 0);
}

bool RouteProcessorsModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if(parent.isValid())
		return false;

	if(row == 1)
		return (m_route->remove_processor(m_processors.at(row)->processor()) == 0);
	else
	{
		ARDOUR::Route::ProcessorList toRemove;
		for(int i = 0; i < count; ++i)
			toRemove.push_back(m_processors.at(row + i)->processor());
		return (m_route->remove_processors(toRemove) == 0);
	}
}

void RouteProcessorsModel::handleProcessorChanges(ARDOUR::RouteProcessorChange change)
{
	// TODO: More fine-grained
	beginResetModel();
	for(auto& p: m_processors)
		p->deleteLater();
	m_processors.clear();

	populate();

	endResetModel();
}

void RouteProcessorsModel::populate()
{
	m_route->foreach_processor([&](auto processor){
		m_processors.append(Processor::create(this, processor.lock()));
	});
}

void RouteProcessorsModel::removeProcessor(QObject* processor)
{
	for(int i = 0; i < m_processors.size(); ++i)
	{
		if(m_processors.at(i).get() == processor)
		{
			beginRemoveRows(QModelIndex(), i, i);
			m_processors.remove(i);
			endRemoveRows();
			break;
		}
	}
}
