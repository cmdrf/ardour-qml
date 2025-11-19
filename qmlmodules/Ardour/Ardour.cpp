
#include "Ardour.h"
#include "pbd/basename.h"

#include <ardour/audio_backend.h>
#include <ardour/filename_extensions.h>
#include <ardour/session.h>
#include <ardour/session_event.h>
#include <pbd/receiver.h>
#include <pbd/transmitter.h>
#include <pbd/event_loop.h>

#include <QCoreApplication>
#include <QtEnvironmentVariables>
#include <glibmm.h>

#include <functional>
#include <string>

class LogReceiver : public Receiver
{
protected:
	void receive (Transmitter::Channel chn, const char* str) final
	{
		const char* prefix = "";

		switch (chn)
		{
			case Transmitter::Debug:
				qDebug() << str;
				return;
			case Transmitter::Info:
				qInfo() << str;
				return;
			case Transmitter::Warning:
				qWarning() << str;
				break;
			case Transmitter::Error:
				qCritical() << str;
				break;
			case Transmitter::Fatal:
				qFatal() << str;
				break;
			case Transmitter::Throw:
				/* this isn't supposed to happen */
				QCoreApplication::exit(1);
		}

		if (chn == Transmitter::Fatal)
		{
			QCoreApplication::exit (9);
		}
	}
};

class MyEventLoop : public sigc::trackable, public PBD::EventLoop
{
public:
	MyEventLoop() : EventLoop (std::string("asd"))
	{
		run_loop_thread = Glib::Threads::Thread::self ();
	}

	bool call_slot (InvalidationRecord* ir, const std::function<void ()>& f)
	{
		if (Glib::Threads::Thread::self () == run_loop_thread)
		{
			qInfo() << string_compose ("%1/%2 direct dispatch of call slot via functor @ %3, invalidation %4\n", event_loop_name (), pthread_name (), &f, ir);
			f ();
		}
		else
		{
			qInfo() << string_compose ("%1/%2 queue call-slot using functor @ %3, invalidation %4\n", event_loop_name (), pthread_name (), &f, ir);
			//qAssert (!ir);
			f (); // XXX TODO, queue and process during run ()
		}
		return true;
	}

	void run ()
	{
		; // TODO process Events, if any
	}

	Glib::Threads::RWLock& slot_invalidation_rwlock ()
	{
		return request_buffer_map_lock;
	}

private:
	Glib::Threads::Thread* run_loop_thread;
	Glib::Threads::RWLock  request_buffer_map_lock;
};

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

	static LogReceiver logReceiver;
	static MyEventLoop eventLoop;

	PBD::EventLoop::set_event_loop_for_thread (&eventLoop);
	ARDOUR::SessionEvent::create_per_thread_pool ("lua", 4096);


	logReceiver.listen_to (PBD::warning);
	logReceiver.listen_to (PBD::error);
	logReceiver.listen_to (PBD::fatal);
}

Ardour::~Ardour()
{
	closeSession();
	ARDOUR::AudioEngine::instance ()->stop ();
	ARDOUR::cleanup ();
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
	if(!prepareEngine ())
		return false;

	std::string state = snapshotName.toStdString();
	std::string dirStr = dir.toStdString();
	if (state.empty ())
		state = ARDOUR::Session::get_snapshot_from_instant (dir.toStdString());

	if (state.empty ())
		state = PBD::basename_nosuffix (dirStr);

	float sr;
	ARDOUR::SampleFormat sf;
	std::string v;
	std::string s = Glib::build_filename (dirStr, state + ARDOUR::statefile_suffix);
	if (!Glib::file_test (dirStr, Glib::FILE_TEST_EXISTS))
	{
		qWarning() << "Cannot find session: " << s << "\n";
		return 0;
	}

	if(ARDOUR::Session::get_info_from_path (s, sr, sf, v) != 0)
	{
		qWarning() << "Cannot get samplerate from session.\n";
		return false;
	}

	if(!startEngine(sr))
		return false;

	ARDOUR::AudioEngine* engine  = ARDOUR::AudioEngine::instance ();
	ARDOUR::Session*     session = new ARDOUR::Session (*engine, dirStr, state);
	m_session = new Session(this, session);
	Q_EMIT sessionChanged();
	return true;
}

bool Ardour::loadSession(const QUrl& ardourFile)
{
	QString file = ardourFile.toLocalFile();
	// Split into directory and filename:
	QString dir = QFileInfo(file).absolutePath();
	QString snapshotName = QFileInfo(file).completeBaseName();
	return loadSession(dir, snapshotName);
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
