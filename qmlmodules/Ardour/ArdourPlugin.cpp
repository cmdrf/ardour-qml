#include "ArdourPlugin.h"


ArdourPlugin::ArdourPlugin(QObject *parent)
	: QQmlEngineExtensionPlugin{parent}
{

}

void ArdourPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
	Q_UNUSED(uri);
}
