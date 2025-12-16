#ifndef SESSIONOBJECT_H
#define SESSIONOBJECT_H

#include "StatefulDestructible.h"

#include <ardour/session_object.h>

/// Named object associated with a session
class SessionObject : public StatefulDestructible
{
	Q_OBJECT

	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)

public:
	explicit SessionObject(QObject *parent, std::shared_ptr<ARDOUR::SessionObject>);

	std::shared_ptr<ARDOUR::SessionObject> sessionObject() {return std::dynamic_pointer_cast<ARDOUR::SessionObject>(m_stateful);}
	const std::shared_ptr<ARDOUR::SessionObject> sessionObject() const {return std::dynamic_pointer_cast<ARDOUR::SessionObject>(m_stateful);}

	QString name() const;
	void setName(const QString& newName);

Q_SIGNALS:
	void nameChanged();

private Q_SLOTS:
	void updateProperties(const PBD::PropertyChange& change);
};

#endif // SESSIONOBJECT_H
