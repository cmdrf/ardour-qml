#ifndef STATEFUL_H
#define STATEFUL_H

#include <pbd/stateful.h>

#include <QObject>
#include <QVariantMap>

class Stateful : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QVariantMap properties READ properties NOTIFY propertiesChanged FINAL)

	/// Writable extra properties being serialized into the project file
	/** This uses the extra_xml mechanism of Ardour. */
	Q_PROPERTY(QVariantMap extra READ extra WRITE setExtra NOTIFY extraChanged FINAL)

public:
	Stateful(QObject* parent, std::shared_ptr<PBD::Stateful> stateful);

	const QVariantMap& properties();

	const QVariantMap& extra();
	void setExtra(const QVariantMap& newExtra);

Q_SIGNALS:
	/// Signal to QML that properties changed
	void propertiesChanged();

	void extraChanged();

private Q_SLOTS:
	/// Called from PBD when one or more properties changed
	void updateProperties(const PBD::PropertyChange& change);

protected:
	std::shared_ptr<PBD::Stateful> m_stateful;

private:
	QVariantMap m_properties;
	QVariantMap m_extra;
};

#endif // STATEFUL_H
