#include "RouteProcessorsModel.h"

#include <ardour/send.h>
#include <ardour/plugin_insert.h>
#include <ardour/internal_send.h>


RouteProcessorsModel::RouteProcessorsModel(QObject* parent, std::shared_ptr<ARDOUR::Route> route) :
	QAbstractListModel(parent),
	m_route(route)
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(route->processors_changed, this, &RouteProcessorsModel::handleProcessorChanges);

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
			if(send == m_route->monitor_send())
				return ""; // Monitor send is not an accessible send
			else
				return "send";
		}
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

void RouteProcessorsModel::handleProcessorChanges(ARDOUR::RouteProcessorChange change)
{
	// TODO: More fine-grained
	beginResetModel();
	for(auto& p: m_processors)
		p->deleteLater();
	m_processors.clear();

	m_route->foreach_processor([&](auto processor){
		// TODO: Create subclasses of Processor here (Send, PluginInsert):
		m_processors.append(new Processor(this, processor.lock()));
	});

	endResetModel();
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
