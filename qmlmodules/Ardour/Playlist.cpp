#include "Playlist.h"
#include "AudioRegion.h"
#include "MidiRegion.h"
#include "QtBridgeUi.h"
#include "Region.h"

#include <ardour/midi_region.h>
#include <ardour/playlist.h>
#include <ardour/region.h>

Playlist::Playlist(QObject *parent) :
	QAbstractListModel{parent}
{

}

void Playlist::setPlaylist(std::shared_ptr<ARDOUR::Playlist> playlist)
{
	if(m_playlistAddRegionConnection)
	{
		m_playlistAddRegionConnection->disconnect();
		m_playlistAddRegionConnection.reset();
	}

	if(m_playlistRemoveRegionConnection)
	{
		m_playlistRemoveRegionConnection->disconnect();
		m_playlistRemoveRegionConnection.reset();
	}

	m_playlist = playlist;

	QtBridgeUi& b = QtBridgeUi::instance();
	b.connect(m_playlist->RegionAdded, this, &Playlist::_addRegion);
	b.connect(m_playlist->RegionRemoved, this, &Playlist::_removeRegion);


	bool reset = false;
	if(!m_regions.isEmpty())
	{
		reset = true;
		beginResetModel();
		for(auto & region: m_regions)
			delete region;
	}

	auto regionList = m_playlist->region_list();
	for(const auto &region : *regionList)
	{
		Region* newRegion = nullptr;
		if(auto audioRegion = std::dynamic_pointer_cast<ARDOUR::AudioRegion>(region))
			newRegion = new AudioRegion(this, audioRegion);
		else if(auto midiRegion = std::dynamic_pointer_cast<ARDOUR::MidiRegion>(region))
			newRegion = new MidiRegion(this, midiRegion);
		else
			newRegion = new Region(this, region);
		m_regions.append(newRegion);
	}

	if(reset)
		endResetModel();
	Q_EMIT regionsChanged();
}

int Playlist::rowCount(const QModelIndex& parent) const
{
	return m_regions.size();
}

QVariant Playlist::data(const QModelIndex& index, int role) const
{
	if(!checkIndex(index, QAbstractItemModel::CheckIndexOption::IndexIsValid | QAbstractItemModel::CheckIndexOption::ParentIsInvalid))
		return QVariant();

	if(role == RegionRole)
	{
		Region* r = m_regions.at(index.row());
		if(AudioRegion* ar = dynamic_cast<AudioRegion*>(r))
			return QVariant::fromValue(ar);
		else if(MidiRegion* mr = dynamic_cast<MidiRegion*>(r))
			return QVariant::fromValue(mr);
		else
			return QVariant::fromValue(r);
	}
	return QVariant();
}

QHash<int, QByteArray> Playlist::roleNames() const
{
	static const QHash<int, QByteArray> roles{
		{RegionRole, "region"}
	};
	return roles;
}

void Playlist::addRegion(Region* region, const TimePos& position, float times, bool autoPartition)
{
	m_playlist->add_region(region->region(), position, times, autoPartition);
}

void Playlist::removeRegion(Region* region)
{
	m_playlist->remove_region(region->region());
}

void Playlist::replaceRegion(Region* old, Region* newr, const TimePos& pos)
{
	m_playlist->replace_region(old->region(), newr->region(), pos);
}

void Playlist::splitRegion(Region* region, const TimePos& position)
{
	m_playlist->split_region(region->region(), position);
}

void Playlist::split(const TimePos& at)
{
	m_playlist->split(at);
}

void Playlist::shift(const TimePos& at, const TimeCount& distance, bool moveIntersected)
{
	m_playlist->shift(at, distance, moveIntersected);
}

void Playlist::partition(const TimePos& start, const TimePos& end, bool cut)
{
	m_playlist->partition(start, end, cut);
}

void Playlist::duplicate(Region* region, TimePos& position, float times)
{
	ARDOUR::timepos_t pos = position;
	m_playlist->duplicate(region->region(), pos, times);
	position = pos;
}

void Playlist::duplicate(Region* region, TimePos& position, const TimeCount& gap, float times)
{
	ARDOUR::timepos_t pos = position;
	m_playlist->duplicate(region->region(), pos, gap, times);
	position = pos;
}

void Playlist::duplicateUntil(Region* region, TimePos& position, const TimeCount& gap, const TimePos& end)
{
	ARDOUR::timepos_t pos = position;
	m_playlist->duplicate_until(region->region(), pos, gap, end);
	position = pos;
}

void Playlist::nudgeAfter(const TimePos& start, const TimeCount& distance, bool forwards)
{
	m_playlist->nudge_after(start, distance, forwards);
}

void Playlist::uncombine(Region* region)
{
	m_playlist->uncombine(region->region());
}

void Playlist::shuffle(Region* region, int dir)
{
	m_playlist->shuffle(region->region(), dir);
}

void Playlist::ripple(const TimePos& at, const TimeCount& distance, QList<Region*>* exclude)
{
	ARDOUR::RegionList excludeRegions;
	if(exclude)
	{
		for(Region* r : *exclude)
			excludeRegions.push_back(r->region());
	}
	m_playlist->ripple(at, distance, &excludeRegions);
}

void Playlist::ripple(const TimePos& at, const TimeCount& distance, Region* exclude)
{
	m_playlist->ripple(at, distance, exclude->region());
}

int Playlist::paste(QSharedPointer<Playlist> pl, const TimePos& position, float times)
{
	return m_playlist->paste(pl->m_playlist, position, times);
}

void Playlist::_addRegion(std::weak_ptr<ARDOUR::Region> region)
{
	if(region.expired())
		return;

	auto regionPtr = region.lock();
	int i = m_regions.size();
	beginInsertRows(QModelIndex(), i, i);
	m_regions.append(new Region(this, regionPtr));
	endInsertRows();
	Q_EMIT regionsChanged();
}

void Playlist::_removeRegion(std::weak_ptr<ARDOUR::Region> region)
{
	if(region.expired())
		return;

	auto regionPtr = region.lock();
	qsizetype index = -1;
	for(qsizetype i = 0; i < m_regions.length(); ++i)
	{
		if(m_regions.at(i)->region() == regionPtr)
		{
			index = i;
			break;
		}
	}

	if(index < 0)
		return;

	beginRemoveRows(QModelIndex(), index, index);
	delete m_regions.at(index);
	m_regions.removeAt(index);
	endRemoveRows();
	Q_EMIT regionsChanged();
}
