#include "Track.h"
#include "QtBridgeUi.h"


Track::Track(QObject* parent, std::shared_ptr<ARDOUR::Track> track)
	: Route{parent, track}
{
	QtBridgeUi& b = QtBridgeUi::instance();
	b.connect(track->FreezeChange, this, &Track::freezeStateChanged);
	b.connect(track->PlaylistChanged, this, &Track::updatePlaylist);
	b.connect(track->PlaylistAdded, this, &Track::updatePlaylist);

	m_playlist.setPlaylist(track->playlist());
}

Track::FreezeState Track::freezeState() const
{
	return static_cast<FreezeState>(track()->freeze_state());
}

const ARDOUR::Track* Track::track() const
{
	Q_ASSERT(m_route->is_track());
	return static_cast<ARDOUR::Track*>(m_route.get());
}

ARDOUR::Track* Track::track()
{
	Q_ASSERT(m_route->is_track());
	return static_cast<ARDOUR::Track*>(m_route.get());
}

Track::AlignStyle Track::alignStyle() const
{
	return static_cast<AlignStyle>(track()->alignment_style());
}

void Track::setAlignStyle(const AlignStyle& newAlignStyle)
{
	track()->set_align_style(static_cast<ARDOUR::AlignStyle>(newAlignStyle));
}

void Track::updatePlaylist()
{
	m_playlist.setPlaylist(track()->playlist());
}
