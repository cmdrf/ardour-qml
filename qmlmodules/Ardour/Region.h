#ifndef REGION_H
#define REGION_H

#include "Stateful.h"
#include "TimeCount.h"
#include "TimePos.h"

#include <ardour/region.h>

class Region : public Stateful
{
	Q_OBJECT
	Q_PROPERTY(TimePos position READ position WRITE setPosition NOTIFY positionChanged FINAL)
	Q_PROPERTY(TimePos start READ start WRITE setStart NOTIFY startChanged FINAL)
	Q_PROPERTY(TimeCount length READ length WRITE setLength NOTIFY lengthChanged FINAL)
	Q_PROPERTY(TimePos syncPosition READ syncPosition WRITE setSyncPosition NOTIFY syncPositionChanged FINAL)
	Q_PROPERTY(TimePos ancestralStart READ ancestralStart NOTIFY ancestralStartChanged FINAL)
	Q_PROPERTY(TimeCount ancestralLength READ ancestralLength NOTIFY ancestralLengthChanged FINAL)
	Q_PROPERTY(bool hidden READ hidden WRITE setHidden NOTIFY hiddenChanged FINAL)
	Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged FINAL)
	Q_PROPERTY(bool wholeFile READ wholeFile WRITE setWholeFile NOTIFY wholeFileChanged FINAL)
	Q_PROPERTY(bool automatic READ automatic WRITE setAutomatic NOTIFY automaticChanged FINAL)
	Q_PROPERTY(bool opaque READ opaque WRITE setOpaque NOTIFY opaqueChanged FINAL)
	Q_PROPERTY(bool locked READ locked WRITE setLocked NOTIFY lockedChanged FINAL)
	Q_PROPERTY(bool videoLocked READ videoLocked WRITE setVideoLocked NOTIFY videoLockedChanged FINAL)
	Q_PROPERTY(bool positionLocked READ positionLocked WRITE setPositionLocked NOTIFY positionLockedChanged FINAL)
	Q_PROPERTY(quint64 layeringIndex READ layeringIndex WRITE setLayeringIndex NOTIFY layeringIndexChanged FINAL)
	Q_PROPERTY(QString tags READ tags WRITE setTags NOTIFY tagsChanged FINAL)
	Q_PROPERTY(quint64 regionGroup READ regionGroup WRITE setRegionGroup NOTIFY regionGroupChanged FINAL)
	Q_PROPERTY(float stretch READ stretch NOTIFY stretchChanged FINAL)
	Q_PROPERTY(float shift READ shift NOTIFY shiftChanged FINAL)
	Q_PROPERTY(DataType dataType READ dataType CONSTANT FINAL)

	QML_ELEMENT
	QML_UNCREATABLE("")
	Q_DISABLE_COPY(Region)

public:
	enum DataType
	{
		AudioType,
		MidiType,
		UnknownType
	};
	Q_ENUM(DataType)

	explicit Region(QObject *parent, std::shared_ptr<ARDOUR::Region> region);

	TimePos position() const {return region()->position();}
	TimePos start() const {return region()->start();}
	TimeCount length() const {return region()->length();}
	TimePos syncPosition() const {return region()->sync_position();}
	TimePos ancestralStart() const {return region()->ancestral_start();}
	TimeCount ancestralLength() const {return region()->ancestral_length();}
	bool hidden() const {return region()->hidden();}
	bool muted() const {return region()->muted();}
	bool wholeFile() const {return region()->whole_file();}
	bool automatic() const {return region()->automatic();}
	bool opaque() const {return region()->opaque();}
	bool locked() const {return region()->locked();}
	bool videoLocked() const {return region()->video_locked();}
	bool positionLocked() const {return region()->position_locked();}
	quint64 layeringIndex() const {return region()->layering_index();}
	QString tags() const {return QString::fromStdString(region()->tags());}
	quint64 regionGroup() const {return region()->region_group();}
	float stretch() const {return region()->stretch();}
	float shift() const {return region()->shift();}
	DataType dataType() const;

	std::shared_ptr<ARDOUR::Region> region() {return std::dynamic_pointer_cast<ARDOUR::Region>(m_stateful);}
	const std::shared_ptr<ARDOUR::Region> region() const {return std::dynamic_pointer_cast<ARDOUR::Region>(m_stateful);}

public Q_SLOTS:
	void setPosition(TimePos pos) {region()->set_position(pos);}
	void setStart(TimePos pos) {region()->set_start(pos);}
	void setLength(TimeCount len) {region()->set_length(len);}
	void setSyncPosition(TimePos pos) {region()->set_sync_position(pos);}
	void setHidden (bool yn) {region()->set_hidden(yn);}
	void setMuted (bool yn) {region()->set_muted(yn);}
	void setWholeFile (bool yn) {region()->set_whole_file(yn);}
	void setAutomatic (bool yn) {region()->set_automatic(yn);}
	void setOpaque (bool yn) {region()->set_opaque(yn);}
	void setLocked (bool yn) {region()->set_locked(yn);}
	void setVideoLocked (bool yn) {region()->set_video_locked(yn);}
	void setPositionLocked (bool yn) {region()->set_position_locked(yn);}
	void setLayeringIndex(quint64 index) {region()->set_layering_index(index);}
	void setTags(const QString& tags) {region()->set_tags(tags.toStdString());}
	void setRegionGroup(quint64 group) {region()->set_region_group(group);}

	void setAncestralData(const TimePos& start, const TimeCount& length, float stretch, float shift);

	// Editing operations:
	void nudgePosition(const TimeCount& t) {region()->nudge_position(t);}
	void moveToNaturalPosition() {region()->move_to_natural_position();}
	void moveStart(const TimeCount& t) {region()->move_start(t);}
	void trimFront(const TimePos& t) {region()->trim_front(t);}
	void trimEnd(const TimePos& t) {region()->trim_end(t);}
	void trimTo(const TimePos& pos, const TimeCount& length) {region()->trim_to(pos, length);}
	// void fadeRange
	void cutFront(const TimePos& newPosition) {region()->cut_front(newPosition);}
	void cutEnd(const TimePos& newPosition) {region()->cut_end(newPosition);}
	void raise() {region()->raise();}
	void lower() {region()->lower();}
	void raiseToTop() {region()->raise_to_top();}
	void lowerToBottom() {region()->lower_to_bottom();}
	void clearSyncPosition() {region()->clear_sync_position();}

Q_SIGNALS:
	void positionChanged();
	void startChanged();
	void lengthChanged();
	void syncPositionChanged();
	void ancestralStartChanged();
	void ancestralLengthChanged();
	void hiddenChanged();
	void mutedChanged();
	void wholeFileChanged();
	void automaticChanged();
	void opaqueChanged();
	void lockedChanged();
	void videoLockedChanged();
	void positionLockedChanged();
	void layeringIndexChanged();
	void tagsChanged();
	void regionGroupChanged();
	void stretchChanged();
	void shiftChanged();

private Q_SLOTS:
	void updateProperties(const PBD::PropertyChange& change);

private:
};

#endif // REGION_H
