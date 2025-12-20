#include "Session.h"
#include "QtBridgeUi.h"

#include <ardour/plugin_insert.h>

Session::Session(QObject* parent, ARDOUR::Session* session) :
	QObject(parent),
	m_session(session),
	m_transportSpeed(session->transport_speed()),
	m_playLoop(session->get_play_loop()),
	m_selection(this, session->selection())

{
	QtBridgeUi& b = QtBridgeUi::instance();
	b.connect(m_session->DirtyChanged, this, &Session::dirtyChanged);
	b.connect(m_session->RecordStateChanged, this, &Session::recordStateChanged);
	b.connect(m_session->TransportStateChange, this, &Session::transportStateChange);
	b.connect(m_session->PositionChanged, this, &Session::transportSampleChanged);
	b.connect(m_session->Located, this, &Session::transportSampleChanged);

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
