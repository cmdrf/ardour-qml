#ifndef PLUGININFO_H
#define PLUGININFO_H

#include <ardour/plugin.h>
#include <QObject>

class PluginInfo
{
	Q_GADGET

	Q_PROPERTY(QString name READ name CONSTANT FINAL)
	Q_PROPERTY(QString category READ category CONSTANT FINAL)
	Q_PROPERTY(QString creator READ creator CONSTANT FINAL)
	Q_PROPERTY(QString path READ path CONSTANT FINAL)

public:
	PluginInfo(std::shared_ptr<ARDOUR::PluginInfo> pluginInfo) : m_pluginInfo(pluginInfo) {}

	std::shared_ptr<ARDOUR::PluginInfo> pluginInfo() {return m_pluginInfo;}
	const std::shared_ptr<ARDOUR::PluginInfo> pluginInfo() const {return m_pluginInfo;}

	QString name() const {return QString::fromStdString(m_pluginInfo->name);}
	QString category() const {return QString::fromStdString(m_pluginInfo->category);}
	QString creator() const {return QString::fromStdString(m_pluginInfo->creator);}
	QString path() const {return QString::fromStdString(m_pluginInfo->path);}
//		ChanCount          n_inputs;
//		ChanCount          n_outputs;
//		ARDOUR::PluginType type;

private:
	std::shared_ptr<ARDOUR::PluginInfo> m_pluginInfo;
};

#endif // PLUGININFO_H
