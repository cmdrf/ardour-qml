#include "ChanCount.h"
#include "Location.h"
#include "Session.h"
#include "TimePos.h"
#include "Track.h"
#include "QtBridgeUi.h"

#include <ardour/plugin_insert.h>
#include <ardour/audio_track.h>
#include <ardour/midi_track.h>

Session::Session(QObject* parent, std::shared_ptr<ARDOUR::Session> session) :
	StatefulDestructible(parent, session),
	m_session(session.get()),
	m_transportSpeed(session->transport_speed()),
	m_playLoop(session->get_play_loop()),
	m_selection(this, session->selection()),
	m_tempoMap(this, session.get())
{
	QtBridgeUi& b = QtBridgeUi::instance();
	b.connect(m_session->DirtyChanged, this, &Session::dirtyChanged);
	b.connect(m_session->RecordStateChanged, this, &Session::recordStateChanged);
	b.connect(m_session->TransportStateChange, this, &Session::transportStateChange);
	b.connect(m_session->PositionChanged, this, &Session::transportSampleChanged);
	b.connect(m_session->Located, this, &Session::transportSampleChanged);
	b.connect(m_session->StartTimeChanged, this, &Session::currentStartChanged);
	b.connect(m_session->EndTimeChanged, this, &Session::currentEndChanged);

	// Locations:
	b.connect(m_session->auto_loop_location_changed, this, &Session::onAutoLoopLocationChanged);
	b.connect(m_session->auto_punch_location_changed, this, &Session::onAutoPunchLocationChanged);

	// Route model:
	ARDOUR::RouteList routes = *m_session->get_routes();
	m_routes.addRoutes(routes); // Intitial populate
	b.connect(m_session->RouteAdded, &m_routes, &RouteModel::addRoutes);

	m_tracks.setSourceModel(&m_routes);
	m_tracks.setFilterRole(RouteModel::IsTrackRole);
	m_tracks.setFilterFixedString("true");
	m_tracks.setSortRole(RouteModel::TrackNumberRole);

	// Timer:
	m_transportPositionQueryTimer.setInterval(100);
	connect(&m_transportPositionQueryTimer, &QTimer::timeout, this, &Session::transportSampleChanged);
}

bool Session::dirty() const
{
	return m_session->dirty();
}

Processor* Session::newPlugin(const PluginInfo& info, const QString& preset)
{
	ARDOUR::PluginPtr p = info.pluginInfo()->load(*m_session);
	if(!p)
		return nullptr;

	if(!preset.isEmpty())
	{
		const ARDOUR::Plugin::PresetRecord *pr = p->preset_by_label (preset.toStdString());
		if (pr)
			p->load_preset (*pr);
	}

	auto pluginInsert = std::shared_ptr<ARDOUR::Processor>(new ARDOUR::PluginInsert(*m_session, *m_session, p));
	// Don't set parent, so QML takes ownership:
	return Processor::create(nullptr, pluginInsert);
}

Track* Session::newAudioTrack(int inputChannels, int outputChannels, RouteGroup* routeGroup, int order, Ardour::TrackMode mode)
{
	std::shared_ptr<ARDOUR::RouteGroup> aRouteGroup(routeGroup ? routeGroup->routeGroup() : nullptr);
	ARDOUR::TrackMode aMode = static_cast<ARDOUR::TrackMode>(mode);

	auto tracks = m_session->new_audio_track(inputChannels, outputChannels, aRouteGroup, 1, std::string(), order, aMode);
	return new Track(nullptr, tracks.front()); // Don't set parent, so QML takes ownership
}

Track* Session::newMidiTrack(const ChanCount& input, const ChanCount& output, bool strictIo, PluginInfo* instrument, void* preset, RouteGroup* routeGroup, int order, Ardour::TrackMode mode, bool inputAutoConnect, bool triggerVisibility)
{
	std::shared_ptr<ARDOUR::RouteGroup> aRouteGroup(routeGroup ? routeGroup->routeGroup() : nullptr);
	ARDOUR::TrackMode aMode = static_cast<ARDOUR::TrackMode>(mode);

	auto tracks = m_session->new_midi_track(
		input.chanCount(),
		output.chanCount(),
		strictIo,
		instrument->pluginInfo(),
		nullptr, // pset
		aRouteGroup,
		1, // How many
		std::string(),
		order,
		aMode,
		inputAutoConnect,
		triggerVisibility);
	return new Track(nullptr, tracks.front()); // Don't set parent, so QML takes ownership
}

Session::RecordState Session::recordState() const
{
	switch(m_session->record_status())
	{
		case ARDOUR::Disabled:
			return Disabled;
		case ARDOUR::Enabled:
			return Enabled;
		case ARDOUR::Recording:
			return Recording;
	}
}

Location* Session::autoLoopLocation()
{
	if(m_autoLoopLocation)
		return m_autoLoopLocation;

	auto autoLoopLocation = m_session->locations()->auto_loop_location();
	// If Ardour has loop location, but we don't create wrapper:
	if(autoLoopLocation)
		m_autoLoopLocation = new Location(this, autoLoopLocation);
	return m_autoLoopLocation;
}

void Session::setAutoLoopLocation(const TimePos& start, const TimePos& end)
{
	ARDOUR::Location* existingLocation = m_session->locations()->auto_loop_location();
	if(existingLocation == nullptr)
	{
		ARDOUR::Location* newLocation = new ARDOUR::Location(*m_session, start, end, "Loop",  ARDOUR::Location::IsAutoLoop);
		m_session->locations()->add(newLocation, true);
		m_session->set_auto_loop_location(newLocation);
	}
	else
	{
		existingLocation->set_hidden(false, this);
		existingLocation->set(start, end);
	}
}

Location* Session::autoPunchLocation()
{
	if(m_autoPunchLocation)
		return m_autoPunchLocation;

	auto autoPunchLocation = m_session->locations()->auto_punch_location();
	// If Ardour has loop location, but we don't create wrapper:
	if(autoPunchLocation)
		m_autoPunchLocation = new Location(this, autoPunchLocation);
	return m_autoPunchLocation;
}

void Session::setAutoPunchLocation(const TimePos& start, const TimePos& end)
{
	ARDOUR::Location* existingLocation = m_session->locations()->auto_punch_location();
	if(existingLocation == nullptr)
	{
		ARDOUR::Location* newLocation = new ARDOUR::Location(*m_session, start, end, "Punch",  ARDOUR::Location::IsAutoPunch);
		m_session->locations()->add(newLocation, true);
		m_session->set_auto_punch_location(newLocation);
	}
	else
	{
		existingLocation->set_hidden(false, this);
		existingLocation->set(start, end);
	}
}

void Session::maybeEnableRecord()
{
	m_session->maybe_enable_record();
}

void Session::disableRecord()
{
	m_session->disable_record(false);
}

void Session::requestRoll()
{
	m_session->request_roll();
}

void Session::requestLocate(qint64 sample)
{
	m_session->request_locate(sample);
}

void Session::requestStop()
{
	m_session->request_stop();
}

void Session::transportStateChange()
{
	float transportSpeed = m_session->transport_speed();
	if(!qFuzzyCompare(transportSpeed, m_transportSpeed))
	{
		m_transportSpeed = transportSpeed;
		if(transportSpeed == 0.0)
			m_transportPositionQueryTimer.stop();
		else
			m_transportPositionQueryTimer.start();

		Q_EMIT transportSpeedChanged();
	}

	bool playLoop = m_session->get_play_loop();
	if( playLoop != m_playLoop)
	{
		m_playLoop = playLoop;
		Q_EMIT playLoopChanged();
	}
}

/// Checks if the location pointer changed and updates the wrapper accordingly.
/** @returns true if the pointer changed, false if only properties changed. */
static bool checkLocation(Location*& locationWrapper, ARDOUR::Location* ardourLocation)
{
	if(ardourLocation == nullptr && locationWrapper != nullptr) // Check if it was removed
	{
		delete locationWrapper;
		locationWrapper = nullptr;
		return true;
	}
	if(ardourLocation != nullptr && locationWrapper == nullptr)
	{
		// Location creation deferred to Session::autoLoopLocation() or Session::autoPunchLocation()
		return true;
	}
	if(locationWrapper != nullptr && locationWrapper->location() != ardourLocation) // Check if pointer changed
	{
		QObject* parent = locationWrapper->parent();
		delete locationWrapper;
		locationWrapper = new Location(parent, ardourLocation);
		return true;
	}
	return false;
}

void Session::onAutoLoopLocationChanged()
{
	/* This gets called on every change of the auto loop location, but we are only interested if
	   the location pointer itself changed. Location properties are handled by Location already. */
	auto autoLoopLocation = m_session->locations()->auto_loop_location();
	if(checkLocation(m_autoLoopLocation, autoLoopLocation))
		Q_EMIT autoLoopLocationChanged();
}

void Session::onAutoPunchLocationChanged()
{
	/* This gets called on every change of the auto punch location, but we are only interested if
	   the location pointer itself changed. Location properties are handled by Location already. */
	auto autoPunchLocation = m_session->locations()->auto_punch_location();
	if(checkLocation(m_autoPunchLocation, autoPunchLocation))
		Q_EMIT autoPunchLocationChanged();
}

TimePos Session::currentEnd() const
{
	return m_session->current_end();
}

void Session::setCurrentEnd(const TimePos& newCurrentEnd)
{
	m_session->set_session_extents(m_session->current_start(), newCurrentEnd);
}

TimePos Session::currentStart() const
{
	return m_session->current_start();
}

void Session::setCurrentStart(const TimePos& newCurrentStart)
{
	m_session->set_session_extents(newCurrentStart, m_session->current_end());
}
