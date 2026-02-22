#include "SessionTest.h"
#include "../qmlmodules/Ardour/Session.h"
#include "../qmlmodules/Ardour/Location.h"
#include "../qmlmodules/Ardour/TimePos.h"
#include "../qmlmodules/Ardour/Ardour.h"

#include <QTest>
#include <QSignalSpy>
#include <QDebug>
#include <QTemporaryDir>

QTEST_MAIN(SessionTest)

void SessionTest::initTestCase()
{
    // Create ArdourApp instance (this initializes ARDOUR)
    m_ardourApp = new ArdourApp();
	m_ardourApp->setSelectedBackend("None (Dummy)");

    // Create a temporary directory for the test session
    m_tempDir = new QTemporaryDir();
	if(!m_tempDir->isValid())
	{
        qWarning() << "Failed to create temporary directory for test session";
        return;
    }

    // Try to create test session using the existing createSession method
	try
	{
		bool sessionCreated = m_ardourApp->createSession(m_tempDir->path() + "/test", "test-session", 48000);
		if(sessionCreated)
		{
            // Get the session for testing
            m_session = m_ardourApp->session();
            QVERIFY(m_session != nullptr);
		}
		else
		{
            qWarning() << "Failed to create test session - some tests will be skipped";
            m_session = nullptr;
        }
	}
	catch (const std::exception& e)
	{
        qCritical() << "Exception caught while creating session:" << e.what();
        m_session = nullptr;
    }
}

void SessionTest::cleanupTestCase()
{
	if(m_ardourApp)
	{
        delete m_ardourApp;
        m_ardourApp = nullptr;
    }

	if(m_tempDir)
	{
        delete m_tempDir;
        m_tempDir = nullptr;
    }
}

void SessionTest::testAutoLoopLocation()
{
	// Test initial state - should be nullptr
    QVERIFY(m_session->autoLoopLocation() == nullptr);

    // Test signal connection
    QSignalSpy spy(m_session, &Session::autoLoopLocationChanged);
    QVERIFY(spy.isValid());

    TimePos start(ARDOUR::timepos_t(0));
    TimePos end(ARDOUR::timepos_t(48000)); // 1 second at 48kHz

    // Set auto loop location
    m_session->setAutoLoopLocation(start, end);

    // Verify signal was emitted
    QTRY_VERIFY(spy.count() >= 1);

    // Verify location was set
    Location* location = m_session->autoLoopLocation();
    QVERIFY(location != nullptr);

	// Test location properties
    QCOMPARE(location->start().samples(), start.samples());
    QCOMPARE(location->end().samples(), end.samples());

}
