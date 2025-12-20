#include "PluginManager.h"
#include "QtBridgeUi.h"

#include <ardour/plugin_manager.h>

#include <QDebug>


PluginManager::PluginManager(QObject *parent)
	: QObject{parent}
{
	QtBridgeUi& b = QtBridgeUi::instance();
	ARDOUR::PluginManager& manager = ARDOUR::PluginManager::instance();

	b.connect(manager.PluginListChanged, this, &PluginManager::updatePluginLists);
}

static void append(QList<PluginInfo>& out, const ARDOUR::PluginInfoList& in)
{
	for(auto& i: in)
		out.append(PluginInfo(i));
}

QList<PluginInfo> PluginManager::allPlugins()
{
	if(m_allPlugins.empty())
	{
		ARDOUR::PluginManager& manager = ARDOUR::PluginManager::instance();
		manager.refresh();
		std::vector<std::shared_ptr<ARDOUR::PluginScanLogEntry>> log;
		manager.scan_log(log);
		for(auto& l: log)
			qDebug() << l->log();

		append(m_allPlugins, manager.ladspa_plugin_info());
		append(m_allPlugins, manager.lua_plugin_info());
		append(m_allPlugins, manager.windows_vst_plugin_info());
		append(m_allPlugins, manager.mac_vst_plugin_info());
		append(m_allPlugins, manager.lxvst_plugin_info());
		append(m_allPlugins, manager.vst3_plugin_info());
		append(m_allPlugins, manager.au_plugin_info());
		append(m_allPlugins, manager.lv2_plugin_info());
	}

	return m_allPlugins;
}

void PluginManager::updatePluginLists()
{
	m_allPlugins.clear();
	Q_EMIT allPluginsChanged();
}
