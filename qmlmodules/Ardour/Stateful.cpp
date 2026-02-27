#include "Stateful.h"
#include "QtBridgeUi.h"

#include <pbd/properties.h>

#include <QJsonObject>
#include <QVariant>
#include <QMetaMethod>

static QVariant toVariant(const PBD::PropertyBase *pb)
{
	if(!pb)
		return QVariant();

	// Try common type specializations
	if(auto pi = dynamic_cast<const PBD::Property<int>*>(pb))
		return QVariant::fromValue(pi->val());
	if(auto pf = dynamic_cast<const PBD::Property<float>*>(pb))
		return QVariant::fromValue(pf->val());
	if(auto pd = dynamic_cast<const PBD::Property<double>*>(pb))
		return QVariant::fromValue(pd->val());
	if(auto pbv = dynamic_cast<const PBD::Property<bool>*>(pb))
		return QVariant::fromValue(pbv->val());
	if(auto ps = dynamic_cast<const PBD::Property<std::string>*>(pb))
		return QString::fromStdString(ps->val());

	// Fallback: Get value as XML property:
	XMLNode node("dummy");
	pb->get_value(node);
	XMLPropertyList properties = node.properties();
	if(properties.empty())
		return QVariant();
	return QString::fromStdString(properties.front()->value());
}

static QVariantMap toVariantMap(const PBD::PropertyList& list)
{
	QVariantMap out;
	for(auto prop: list)
	{
		QString name(prop.second->property_name());
		QVariant value = toVariant(prop.second);
		out.insert(name, value);
	}
	return out;
}

Stateful::Stateful(QObject *parent, std::shared_ptr<PBD::Stateful> stateful) :
	QObject{parent},
	m_stateful(stateful)
{
	Q_ASSERT(stateful);

	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(stateful->PropertyChanged, this, &Stateful::updateProperties);
}

const QVariantMap& Stateful::properties()
{
	// Deferred population:
	if(m_properties.isEmpty())
		m_properties = toVariantMap(m_stateful->properties());

	return m_properties;
}

void Stateful::updateProperties(const PBD::PropertyChange& change)
{
	// Optimization if nothing is connected:
	static const QMetaMethod propertiesChangedSignal = QMetaMethod::fromSignal(&Stateful::propertiesChanged);
	if(isSignalConnected(propertiesChangedSignal))
		return;

	if(m_properties.isEmpty())
		m_properties = toVariantMap(m_stateful->properties());

	for(auto id: change)
	{
		const char* name = g_quark_to_string(id);
		QVariant newValue = toVariant(m_stateful->properties().at(id));
		m_properties[name] = newValue;
	}
	Q_EMIT propertiesChanged();
}

const QVariantMap& Stateful::extra()
{
	if(m_extra.isEmpty())
	{
		auto node = m_stateful->extra_xml("ardour-qml", false);
		if(node)
			m_extra = QJsonValue::fromJson(node->child_content()).toObject().toVariantMap();
	}
	return m_extra;
}

void Stateful::setExtra(const QVariantMap& newExtra)
{
	if (m_extra == newExtra)
		return;
	m_extra = newExtra;
	auto node = m_stateful->extra_xml("ardour-qml", true);
	auto text = node->child("text");
	if(!text)
		text = node->add_child("text");
	text->set_content(QJsonValue::fromVariant(m_extra).toJson().toStdString());
	Q_EMIT extraChanged();
}
