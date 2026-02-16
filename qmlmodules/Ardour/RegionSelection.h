#ifndef REGIONSELECTION_H
#define REGIONSELECTION_H

#include "Region.h"

#include <QObject>

class Playlist;

class RegionSet : public QSet<Region*>
{
	Q_GADGET

public:
	Q_INVOKABLE bool contains(Region* region) {return QSet<Region*>::contains(region);}
};

/// Manages selection state of regions
/** General idea: If the selection changes, the regionsChanged signal is emitted, causing region delegates
	in QML to re-evaluate regionSelection.regions.contains(region). */
class RegionSelection : public QObject
{
	Q_OBJECT
	QML_ELEMENT

	/// Currently selected regions
	Q_PROPERTY(RegionSet regions READ regions NOTIFY regionsChanged FINAL)

public:
	enum SelectionFlag {
		NoUpdate       = 0,
		Clear          = 0b0001,
		Select         = 0b0010,
		Deselect       = 0b0100,
		Toggle         = 0b1000,
		ClearAndSelect = Clear | Select
	};

	Q_DECLARE_FLAGS(SelectionFlags, SelectionFlag)
	Q_FLAG(SelectionFlags)

	explicit RegionSelection(QObject *parent = nullptr);

	const RegionSet& regions() const { return m_regions; }

public Q_SLOTS:
	void clear();

	/// Change selection of single region
	void select(Region* region, SelectionFlags command);

	/// Change selection of all regions within playlist
	void select(Playlist* playlist, SelectionFlags command);

Q_SIGNALS:
	void regionsChanged();

private Q_SLOTS:
	void removeDestroyedRegion(QObject* o);

private:
	void addRegion(Region* region);
	void removeRegion(Region* region);
	void clearRegions();

	RegionSet m_regions;
};

#endif // REGIONSELECTION_H
