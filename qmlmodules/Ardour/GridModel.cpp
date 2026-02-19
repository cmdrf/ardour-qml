#include "GridModel.h"
#include "QtBridgeUi.h"
#include "TimePos.h"

#include <temporal/superclock.h>
#include <temporal/tempo.h>

GridModel::GridModel(QObject *parent)
	: QAbstractListModel{parent}
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(Temporal::TempoMap::MapChanged, this, &GridModel::updateMap);
	updateMap();
}

int GridModel::rowCount(const QModelIndex& parent) const
{
	if(parent.isValid())
		return 0;
	return m_points.size();
}

QVariant GridModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
		return QVariant();
	if(role == TimeRole)
	{
		const auto time = m_points.at(index.row()).time();
		return QVariant::fromValue(TimePos(time));
	}
	return QVariant();
}

QHash<int, QByteArray> GridModel::roleNames() const
{
	static const QHash<int, QByteArray> roles{
		{TimeRole, "time"}
	};
	return roles;
}

void GridModel::setStartSamples(qint64 newStartSamples)
{
	if (m_startSamples == newStartSamples)
		return;
	m_startSamples = newStartSamples;
	Q_EMIT startSamplesChanged();
	maybeUpdateMap();
}

void GridModel::setEndSamples(qint64 newEndSamples)
{
	if (m_endSamples == newEndSamples)
		return;
	m_endSamples = newEndSamples;
	Q_EMIT endSamplesChanged();
	maybeUpdateMap();
}

void GridModel::setBarModulo(int newBarModulo)
{
	if (m_barModulo == newBarModulo)
		return;
	m_barModulo = newBarModulo;
	Q_EMIT barModuloChanged();
	updateMap();
}

void GridModel::setBeatDivision(int newBeatDivision)
{
	if (m_beatDivision == newBeatDivision)
		return;
	m_beatDivision = newBeatDivision;
	Q_EMIT beatDivisionChanged();
	updateMap();
}

void GridModel::updateMap()
{
	beginResetModel();
	m_points.clear();
	if(m_startSamples < m_endSamples)
	{
		// Extend actual extent by length in both directions:
		const int64_t length = m_endSamples - m_startSamples;
		m_actualStartSamples = qMax(m_startSamples - length, 0);
		m_actualEndSamples = m_endSamples + length;

		auto map = Temporal::TempoMap::use();
		const Temporal::superclock_t startTime = Temporal::samples_to_superclock(m_actualStartSamples, TEMPORAL_SAMPLE_RATE);
		const Temporal::superclock_t endTime = Temporal::samples_to_superclock(m_actualEndSamples, TEMPORAL_SAMPLE_RATE);
		map->get_grid(m_points, startTime, endTime, m_barModulo, m_beatDivision);
	}
	endResetModel();
}

void GridModel::maybeUpdateMap()
{
	if(m_startSamples < m_actualStartSamples || m_endSamples > m_actualEndSamples)
		updateMap();
}
