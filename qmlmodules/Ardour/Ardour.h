#ifndef ARDOUR_H
#define ARDOUR_H

#include "Session.h"

#include <QObject>

class Ardour : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	Q_DISABLE_COPY(Ardour)

	Q_PROPERTY(Session* session READ session NOTIFY sessionChanged FINAL)

public:
	explicit Ardour(QObject *parent = nullptr);
	~Ardour();

	Session* session() const;

	Q_INVOKABLE bool createSession(const QString& dir, const QString& snapshotName, uint32_t sampleRate);
	Q_INVOKABLE bool loadSession(const QString& dir, const QString& snapshotName = QString());
	Q_INVOKABLE bool loadSession(const QUrl& ardourFile);
	Q_INVOKABLE void closeSession();

Q_SIGNALS:
	void sessionChanged();

private:
	Session* m_session = nullptr;
};

#endif // ARDOUR_H
