#ifndef ARDOUR_H
#define ARDOUR_H

#include "PluginManager.h"
#include "Session.h"

#include <QObject>
#include <QtQml>

class Session;

class ArdourApp : public QObject
{
	Q_OBJECT
	QML_SINGLETON
	QML_NAMED_ELEMENT(Ardour)
	QML_EXTENDED_NAMESPACE(Ardour)
	Q_DISABLE_COPY(ArdourApp)

	Q_PROPERTY(Session* session READ session NOTIFY sessionChanged FINAL)
	Q_PROPERTY(PluginManager* pluginManager READ pluginManager CONSTANT FINAL)

public:
	explicit ArdourApp(QObject* parent = nullptr);
	~ArdourApp();

	Session* session() const;
	PluginManager* pluginManager() {return m_pluginManager;}

	Q_INVOKABLE bool createSession(const QString& dir, const QString& snapshotName, uint32_t sampleRate);
	Q_INVOKABLE bool loadSession(const QString& dir, const QString& snapshotName = QString());
	Q_INVOKABLE bool loadSession(const QUrl& ardourFile);
	Q_INVOKABLE void closeSession();

Q_SIGNALS:
	void sessionChanged();

private:
	Session* m_session = nullptr;
	PluginManager* m_pluginManager;
};

#endif // ARDOUR_H
