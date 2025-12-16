#include "SessionObject.h"


SessionObject::SessionObject(QObject *parent, std::shared_ptr<ARDOUR::SessionObject> sessionObject)
	: StatefulDestructible{parent, sessionObject}
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(sessionObject->PropertyChanged, this, &SessionObject::updateProperties);
}

QString SessionObject::name() const
{
	return QString::fromStdString(sessionObject()->name());
}

void SessionObject::setName(const QString& newName)
{
	sessionObject()->set_name(newName.toStdString());
}

void SessionObject::updateProperties(const PBD::PropertyChange& change)
{
	for(auto id: change)
	{
		const char* name = g_quark_to_string(id);
		if(strcmp(name, "name") == 0)
		{
			Q_EMIT nameChanged();
			break;
		}
	}
}
