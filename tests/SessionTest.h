#ifndef SESSIONTEST_H
#define SESSIONTEST_H

#include <QObject>
#include <QTest>
#include <QTemporaryDir>

class ArdourApp;
class Session;

class SessionTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
	void cleanupTestCase();
	void testAutoLoopLocation();

private:
    ArdourApp* m_ardourApp = nullptr;
    Session* m_session = nullptr;
    QTemporaryDir* m_tempDir = nullptr;
};

#endif // SESSIONTEST_H
