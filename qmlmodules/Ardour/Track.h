#ifndef TRACK_H
#define TRACK_H

#include "Route.h"
#include "Playlist.h"

#include <ardour/track.h>

class Track : public Route
{
	Q_OBJECT
	Q_PROPERTY(FreezeState freezeState READ freezeState NOTIFY freezeStateChanged FINAL)
	Q_PROPERTY(Ardour::AlignStyle alignStyle READ alignStyle WRITE setAlignStyle NOTIFY alignStyleChanged FINAL)
	Q_PROPERTY(Playlist* playlist READ playlist CONSTANT)

public:
	enum FreezeState
	{
		NoFreeze = ARDOUR::Track::NoFreeze,
		Frozen = ARDOUR::Track::Frozen,
		UnFrozen = ARDOUR::Track::UnFrozen
	};
	Q_ENUM(FreezeState);

	explicit Track(QObject* parent, std::shared_ptr<ARDOUR::Track> track);

	FreezeState freezeState() const;
	Ardour::AlignStyle alignStyle() const;
	Playlist* playlist() {return &m_playlist;}

	void setAlignStyle(const Ardour::AlignStyle& newAlignStyle);

Q_SIGNALS:
	void freezeStateChanged();
	void alignStyleChanged();

private Q_SLOTS:
	void updatePlaylist();

private:
	const ARDOUR::Track* track() const;
	ARDOUR::Track* track();
	Playlist m_playlist;
};

#endif // TRACK_H
