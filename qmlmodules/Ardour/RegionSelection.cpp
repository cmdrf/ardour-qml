#include "RegionSelection.h"
#include "Playlist.h"

RegionSelection::RegionSelection(QObject *parent)
	: QObject{parent}
{

}

void RegionSelection::clear()
{
	m_regions.clear();
	Q_EMIT regionsChanged();
}

void RegionSelection::select(Region* region, SelectionFlags command)
{
	bool changed = false;
	if(command & SelectionFlag::Clear && !m_regions.isEmpty())
	{
		clearRegions();
		changed = true;
	}

	if(command & SelectionFlag::Toggle)
	{
		if(m_regions.contains(region))
			removeRegion(region);
		else
			addRegion(region);
		changed = true;
	}

	if(command & SelectionFlag::Select && !m_regions.contains(region))
	{
		addRegion(region);
		changed = true;
	}

	if(command & SelectionFlag::Deselect && m_regions.contains(region))
	{
		removeRegion(region);
		changed = true;
	}

	if(changed)
		Q_EMIT regionsChanged();
}

void RegionSelection::select(Playlist* playlist, SelectionFlags command)
{
	// Change selection for all regions in playlist
	bool changed = false;
	if(command & SelectionFlag::Clear && !m_regions.isEmpty())
	{
		clearRegions();
		changed = true;
	}

	if(command & SelectionFlag::Toggle && !playlist->regions().isEmpty())
	{
		for(auto& r: playlist->regions())
		{
			if(m_regions.contains(r))
				removeRegion(r);
			else
				addRegion(r);
		}
		changed = true;
	}

	if(command & SelectionFlag::Select)
	{
		for(auto& r: playlist->regions())
		{
			if(!m_regions.contains(r))
			{
				addRegion(r);
				changed = true;
			}
		}
	}

	if(command & SelectionFlag::Deselect)
	{
		for(auto& r: playlist->regions())
		{
			if(m_regions.contains(r))
			{
				removeRegion(r);
				changed = true;
			}
		}
	}

	if(changed)
		Q_EMIT regionsChanged();
}

void RegionSelection::removeDestroyedRegion(QObject* o)
{
	Region* region = qobject_cast<Region*>(o);
	Q_ASSERT(region);

	if(m_regions.contains(region))
	{
		// No need to disconnect here. The sender is about to disappear anyway.
		m_regions.remove(region);
		Q_EMIT regionsChanged();
	}
}

void RegionSelection::addRegion(Region* region)
{
	if(region)
	{
		connect(region, &QObject::destroyed, this, &RegionSelection::removeDestroyedRegion);
		m_regions.insert(region);
	}
}

void RegionSelection::removeRegion(Region* region)
{
	if(region)
	{
		disconnect(region, &QObject::destroyed, this, &RegionSelection::removeDestroyedRegion);
		m_regions.remove(region);
	}
}

void RegionSelection::clearRegions()
{
	for(auto& r: std::as_const(m_regions))
		disconnect(r, &QObject::destroyed, this, &RegionSelection::removeDestroyedRegion);
	m_regions.clear();
}
