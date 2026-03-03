#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <ardour/region.h>
#include <QAbstractListModel>

class Region;
class TimePos;
class TimeCount;

class Playlist : public QAbstractListModel
{
	Q_OBJECT

	/// Alternative way to access regions
	Q_PROPERTY(QVector<Region*> regions READ regions NOTIFY regionsChanged FINAL)
	Q_PROPERTY(bool empty READ empty NOTIFY emptyChanged FINAL)

public:
	enum
	{
		RegionRole = Qt::UserRole
	};

	explicit Playlist(QObject *parent = nullptr);

	void setPlaylist(std::shared_ptr<ARDOUR::Playlist> playlist);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

	const QVector<Region*>& regions() const {return m_regions;}

	bool empty() const {return m_empty;}

public Q_SLOTS:
	void addRegion (Region*, TimePos const & position, float times = 1, bool autoPartition = false);
	void removeRegion (Region*);
	//void get_equivalent_regions (std::shared_ptr<Region>, std::vector<std::shared_ptr<Region> >&);

	//void get_region_list_equivalent_regions (std::shared_ptr<Region>, std::vector<std::shared_ptr<Region> >&);
	//void get_source_equivalent_regions (std::shared_ptr<Region>, std::vector<std::shared_ptr<Region> >&);
	void replaceRegion (Region* old, Region* newr, TimePos const & pos);
	void splitRegion (Region*, TimePos const & position);
	void split (TimePos const & at);
	void shift (TimePos const & at, TimeCount const & distance, bool moveIntersected);
	void partition (TimePos const & start, TimePos const & end, bool cut = false);
	void duplicate (Region*, TimePos & position, float times);
	void duplicate (Region*, TimePos & position, const TimeCount& gap, float times);
	void duplicateUntil (Region*, TimePos & position, TimeCount const & gap, TimePos const & end);
//	void duplicateRange (TimelineRange&, float timesTimePos
//	void duplicateRanges (std::list<TimelineRange>&, float times);
	void nudgeAfter (TimePos const & start, TimeCount const & distance, bool forwards);
//	virtual std::shared_ptr<Region> combine (const RegionList&, std::shared_ptr<Track>);
	virtual void uncombine (Region*);
//	void fadeRange (std::list<TimelineRange>&);
//	void removeGaps (TimePos const & gap_threshold, TimePos const & leave_gap, std::function<void (timepos_t, TimeCount)> gap_callback);

	void shuffle (Region*, int dir);

	void ripple (TimePos const & at, TimeCount const & distance, QList<Region*>* exclude);
	void ripple (TimePos const & at, TimeCount const & distance, Region* exclude) ;

//	QSharedPointer<Playlist> cut  (std::list<TimelineRange>&);
//	QSharedPointer<Playlist> copy (std::list<TimelineRange>&);
	int paste (QSharedPointer<Playlist>, TimePos const & position, float times);

Q_SIGNALS:
	void regionsChanged();
	void emptyChanged();

private Q_SLOTS:
	void _addRegion(std::weak_ptr<ARDOUR::Region>);
	void _removeRegion(std::weak_ptr<ARDOUR::Region>);

private:
	void checkEmpty();

	std::shared_ptr<ARDOUR::Playlist> m_playlist;
	std::shared_ptr<PBD::ScopedConnection> m_playlistAddRegionConnection;
	std::shared_ptr<PBD::ScopedConnection> m_playlistRemoveRegionConnection;
	QVector<Region*> m_regions;
	bool m_empty = false;
};

#endif // PLAYLIST_H
