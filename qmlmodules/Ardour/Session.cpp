#include "Session.h"
#include "QtBridgeUi.h"

#include <ardour/session.h>

Session::Session(QObject* parent, ARDOUR::Session* session) :
	QObject(parent),
	m_session(session),
	m_transportSpeed(session->transport_speed()),
	m_playLoop(session->get_play_loop())

{
	QtBridgeUi::instance().connect(m_session->DirtyChanged, this, SIGNAL(dirty()));
	QtBridgeUi::instance().connect(m_session->RecordStateChanged, this, SIGNAL(recordStateChanged()));
	QtBridgeUi::instance().connect(m_session->TransportStateChange, this, SLOT(transportStateChange()));
}

Session::~Session() {}

bool Session::dirty() const
{
	return m_session->dirty();
}

Session::RecordState Session::recordState() const
{
	switch(m_session->record_status())
	{
		case ARDOUR::Session::Disabled:
			return Disabled;
		case ARDOUR::Session::Enabled:
			return Enabled;
		case ARDOUR::Session::Recording:
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
		Q_EMIT transportSpeedChanged();
	}

	bool playLoop = m_session->get_play_loop();
	if( playLoop != m_playLoop)
	{
		m_playLoop = playLoop;
		Q_EMIT playLoopChanged();
	}
}




