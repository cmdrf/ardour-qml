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
	auto point = m_points.at(index.row());
	if(role == TimeRole)
		return QVariant::fromValue(TimePos(point.time()));
	else if(role == BbtBarsRole)
		return point.bbt().bars;
	else if(role == BbtBeatsRole)
		return point.bbt().beats;
	else if(role == BbtTicksRole)
		return point.bbt().ticks;
	return QVariant();
}

QHash<int, QByteArray> GridModel::roleNames() const
{
	static const QHash<int, QByteArray> roles{
		{TimeRole, "time"},
		{BbtBarsRole, "bbtBars"},
		{BbtBeatsRole, "bbtBeats"},
		{BbtTicksRole, "bbtTicks"}
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
	if(newBarModulo < 0)
	{
		qWarning("barModulo can't be negative");
		return;
	}

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
		const int64_t actualLength = qMin(m_actualEndSamples - m_actualStartSamples, 48000 * 300 * (m_barModulo+1)); // Excessive length safeguard
		const int64_t actualActualEnd = m_actualStartSamples + actualLength;

		auto map = Temporal::TempoMap::fetch();
		const Temporal::superclock_t startTime = Temporal::samples_to_superclock(m_actualStartSamples, TEMPORAL_SAMPLE_RATE);
		const Temporal::superclock_t endTime = Temporal::samples_to_superclock(actualActualEnd, TEMPORAL_SAMPLE_RATE);
		map->get_grid(m_points, startTime, endTime, m_barModulo, m_beatDivision);
	}
	endResetModel();
}

void GridModel::maybeUpdateMap()
{
	if(m_startSamples < m_actualStartSamples || m_endSamples > m_actualEndSamples)
		updateMap();
}
