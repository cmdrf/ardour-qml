#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "PluginInfo.h"

#include <QObject>

class PluginManager : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QList<PluginInfo> allPlugins READ allPlugins NOTIFY allPluginsChanged FINAL)

public:
	explicit PluginManager(QObject* parent = nullptr);

	QList<PluginInfo> allPlugins();

Q_SIGNALS:
	void allPluginsChanged();

private Q_SLOTS:
	void updatePluginLists();

private:
	QList<PluginInfo> m_allPlugins;
};

#endif // PLUGINMANAGER_H
