#ifndef ARDOURPLUGIN_H
#define ARDOURPLUGIN_H

#include <QQmlEngineExtensionPlugin>

class ArdourPlugin : public QQmlEngineExtensionPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID QQmlEngineExtensionInterface_iid)

public:
	explicit ArdourPlugin(QObject *parent = nullptr);

	void initializeEngine(QQmlEngine *engine, const char *uri) override;

};

#endif // ARDOURPLUGIN_H
