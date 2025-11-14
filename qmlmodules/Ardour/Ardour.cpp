
#include "Ardour.h"

#include <ardour/audio_backend.h>
#include <ardour/filename_extensions.h>
#include <ardour/session.h>

#include <QCoreApplication>
#include <QtEnvironmentVariables>
#include <glibmm.h>

static bool prepareEngine()
{
	ARDOUR::AudioEngine* engine = ARDOUR::AudioEngine::instance();

	if(!engine->current_backend())
	{
		if (!engine->set_backend("None (Dummy)", "Unit-Test", ""))
		{
			qWarning() << "Cannot create Audio/MIDI engine\n";
			engine->discover_backends();
			auto backends = engine->available_backends();
			qInfo("Available backends:");
			for(auto& backend: backends)
			{
				qInfo() << backend->name;
			}
			return false;
		}
	}

	if (!engine->current_backend ())
	{
		qWarning() << "Cannot create Audio/MIDI engine\n";
		return false;
	}

	if (engine->running ()) {
		engine->stop ();
	}
	return true;
}

static bool startEngine (uint32_t sampleRate)
{
	ARDOUR::AudioEngine* engine = ARDOUR::AudioEngine::instance();

	if(engine->set_sample_rate(sampleRate))
	{
		qWarning() << "Cannot set session's samplerate.\n";
		return false;
	}

	if(engine->start() != 0)
	{
		qWarning() << "Cannot start Audio/MIDI engine\n";
		return false;
	}

	return true;
}

Ardour::Ardour(QObject *parent)
	: QObject{parent}
{
	if(qgetenv("ARDOUR_DLL_PATH") == QByteArray())
		qputenv("ARDOUR_DLL_PATH", ".");

	if (!ARDOUR::init (true, NULL, false))
	{
		QCoreApplication::exit(EXIT_FAILURE);
	}
}

Session* Ardour::session() const
{
	return m_session;
}

bool Ardour::createSession(const QString& dir, const QString& snapshotName, uint32_t sampleRate)
{
	delete m_session;

	if(!prepareEngine())
		return false;

	auto s = QDir(dir).filePath(snapshotName + ARDOUR::statefile_suffix);
	if(QFile::exists(s))
	{
		qWarning() << "Session already exists: " << s << "\n";
		return false;
	}

	if(!startEngine(sampleRate))
		return false;

	ARDOUR::BusProfile busProfile;
	busProfile.master_out_channels = 2;

	ARDOUR::AudioEngine* engine = ARDOUR::AudioEngine::instance();
	ARDOUR::Session* session = new ARDOUR::Session(*engine, dir.toStdString(), snapshotName.toStdString(), &busProfile);
	m_session = new Session(this, session);
	Q_EMIT sessionChanged();
	return true;
}

bool Ardour::loadSession(const QString& dir, const QString& snapshotName)
{

}

void Ardour::closeSession()
{
	if(m_session)
	{
		delete m_session;
		m_session = nullptr;
		Q_EMIT sessionChanged();
	}
}
