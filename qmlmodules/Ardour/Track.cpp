#include "Track.h"
#include "QtBridgeUi.h"


Track::Track(QObject* parent, std::shared_ptr<ARDOUR::Track> track) :
	Route{parent, track},
	m_playlist{new Playlist(this, track->playlist())}
{
	QtBridgeUi& b = QtBridgeUi::instance();
	b.connect(track->FreezeChange, this, &Track::freezeStateChanged);
	b.connect(track->PlaylistChanged, this, &Track::updatePlaylist);
	b.connect(track->PlaylistAdded, this, &Track::updatePlaylist);
}

Track::FreezeState Track::freezeState() const
{
	return static_cast<FreezeState>(track()->freeze_state());
}

const ARDOUR::Track* Track::track() const
{
	Q_ASSERT(route()->is_track());
	return static_cast<ARDOUR::Track*>(route().get());
}

ARDOUR::Track* Track::track()
{
	Q_ASSERT(route()->is_track());
	return static_cast<ARDOUR::Track*>(route().get());
}

Ardour::AlignStyle Track::alignStyle() const
{
	return static_cast<Ardour::AlignStyle>(track()->alignment_style());
}

void Track::setAlignStyle(const Ardour::AlignStyle& newAlignStyle)
{
	track()->set_align_style(static_cast<ARDOUR::AlignStyle>(newAlignStyle));
}

void Track::updatePlaylist()
{
	delete m_playlist;
	m_playlist = new Playlist(this, track()->playlist());
	Q_EMIT playlistChanged();
}
