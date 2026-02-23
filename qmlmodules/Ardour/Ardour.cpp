#include "Ardour.h"
#include "Session.h"

#include <ardour/audio_backend.h>
#include <ardour/filename_extensions.h>
#include <ardour/session.h>
#include <ardour/session_event.h>
#include <pbd/basename.h>
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

static bool prepareEngine(const QString& backend)
{
	ARDOUR::AudioEngine* engine = ARDOUR::AudioEngine::instance();
	return engine->set_backend(backend.toStdString(), "ardour-qml", "") != nullptr;
}

static bool startEngine (uint32_t sampleRate)
{
	ARDOUR::AudioEngine* engine = ARDOUR::AudioEngine::instance();
	std::shared_ptr<ARDOUR::AudioBackend> backend = engine->current_backend();

	const std::string defaultDevice = backend->get_standard_device_name(ARDOUR::AudioBackend::DeviceDefault);
	const std::string noneDevice = backend->get_standard_device_name(ARDOUR::AudioBackend::DeviceNone);
	std::string selectedDevice;
	qInfo() << "Available output devices:";
	auto outputDevices = backend->enumerate_output_devices();
	for(auto& device: outputDevices)
	{
		qInfo() << device.name;
		if(device.name == defaultDevice || (selectedDevice.empty() && device.name != noneDevice))
			selectedDevice = device.name;
	}

	if(backend->set_output_device_name(selectedDevice))
	{
		qWarning() << "Cannot set device to" << selectedDevice;
		return false;
	}

	if(engine->set_sample_rate(sampleRate))
	{
		qWarning() << "Cannot set session's samplerate to" << sampleRate;
		qWarning() << engine->get_last_backend_error();
		return false;
	}

	if(engine->start() != 0)
	{
		qWarning() << "Cannot start Audio/MIDI engine";
		return false;
	}

	return true;
}

ArdourApp::ArdourApp(QObject *parent)
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

	m_pluginManager = new PluginManager(this);

	// Select non-dummy backend if available:
	auto backends = availableBackends();
	backends.removeIf([](auto a){return a == "None (Dummy)";});
	if(backends.isEmpty())
		m_selectedBackend = "None (Dummy)";
	else
		m_selectedBackend = backends.front();
}

ArdourApp::~ArdourApp()
{
	closeSession();
	ARDOUR::cleanup();
}

Session* ArdourApp::session() const
{
	return m_session;
}

bool ArdourApp::createSession(const QString& dir, const QString& snapshotName, uint32_t sampleRate)
{
	delete m_session;

	if(!prepareEngine(m_selectedBackend))
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

bool ArdourApp::loadSession(const QString& dir, const QString& snapshotName)
{
	if(!prepareEngine(m_selectedBackend))
		return false;

	std::string state = snapshotName.toStdString();
	const std::string dirStr = dir.toStdString();
	if(state.empty())
		state = ARDOUR::Session::get_snapshot_from_instant(dirStr);

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

bool ArdourApp::loadSession(const QUrl& ardourFile)
{
	QString file = ardourFile.toLocalFile();
	// Split into directory and filename:
	QString dir = QFileInfo(file).absolutePath();
	QString snapshotName = QFileInfo(file).completeBaseName();
	return loadSession(dir, snapshotName);
}

void ArdourApp::closeSession()
{
	if(m_session)
	{
		ARDOUR::AudioEngine::instance()->remove_session ();
		delete m_session;
		m_session = nullptr;
		ARDOUR::AudioEngine::instance()->stop();
		Q_EMIT sessionChanged();
	}
}

QStringList ArdourApp::availableBackends() const
{
	QStringList backendNames;
	ARDOUR::AudioEngine* engine = ARDOUR::AudioEngine::instance ();
	auto backends = engine->available_backends();

	for(auto& backend: backends)
		backendNames.append(backend->name);

	return backendNames;
}

QString ArdourApp::selectedBackend() const
{
	return m_selectedBackend;
}

void ArdourApp::setSelectedBackend(const QString& newSelectedBackend)
{
	if(newSelectedBackend == m_selectedBackend)
		return;
	m_selectedBackend = newSelectedBackend;
	Q_EMIT selectedBackendChanged();
}
