#include "TempoMap.h"
#include "QtBridgeUi.h"

#include <temporal/tempo.h>

TempoMap::TempoMap(QObject *parent)
	: QAbstractListModel{parent}
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(Temporal::TempoMap::MapChanged, this, &TempoMap::updateMap);
	updateMap();
}

int TempoMap::rowCount(const QModelIndex& parent) const
{
	return m_entries.count();
}

QVariant TempoMap::data(const QModelIndex& index, int role) const
{
	if(!index.isValid() || index.column() != 0 || index.row() >= m_entries.count())
		return QVariant();

	const auto& e = m_entries.at(index.row());
	switch(role)
	{
	case TimeRole:
		return QVariant::fromValue(e.time);
	case TypeRole:
		return e.type;
	case NoteValueRole:
		return e.noteValue;
	case DivisionsPerBarRole:
		return e.divisionsPerBar;
	}
	return QVariant();
}

QHash<int, QByteArray> TempoMap::roleNames() const
{
	static const QHash<int, QByteArray> roles{
		{TimeRole, "time"},
		{TypeRole, "type"},
		{NoteValueRole, "noteValue"},
		{DivisionsPerBarRole, "divisionsPerBar"},
		{NoteTypesPerMinuteRole, "noteTypesPerMinute"}
	};
	return roles;
}

Beats TempoMap::quartersAt(const TimePos& time) const
{
	auto map = Temporal::TempoMap::use();
	return map->quarters_at(time);
}

qint64 TempoMap::sampleAt(const Beats& beats) const
{
	auto map = Temporal::TempoMap::use();
	return map->sample_at(beats);
}

qint64 TempoMap::sampleAt(const TimePos& time) const
{
	auto map = Temporal::TempoMap::use();
	return map->sample_at(time);
}

void TempoMap::updateMap()
{
	beginResetModel();
	m_entries.clear();
	Temporal::TempoMap::Metrics metrics;
	auto map = Temporal::TempoMap::use();
	map->get_metrics(metrics);
	m_entries.reserve(metrics.size());

	for(auto p: metrics)
	{
		Entry entry;
		entry.time = p->time();
		if(const Temporal::MusicTimePoint* mt = dynamic_cast<const Temporal::MusicTimePoint*>(p))
		{
			entry.type = MusicTimePointType;
		}
		else if(const Temporal::MeterPoint* mp = dynamic_cast<const Temporal::MeterPoint*>(p))
		{
			entry.type = MeterPointType;
			entry.noteValue = mp->note_value();
			entry.divisionsPerBar = mp->divisions_per_bar();
		}
		else if(const Temporal::TempoPoint* tp = dynamic_cast<const Temporal::TempoPoint*> (p))
		{
			entry.type = TempoPointType;
			entry.noteValue = tp->note_type();
			entry.noteTypesPerMinute = tp->note_types_per_minute();
		}
		m_entries.append(entry);
	}
	endResetModel();
}
