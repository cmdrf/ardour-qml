#ifndef STATEFUL_H
#define STATEFUL_H

#include <pbd/stateful.h>

#include <QObject>
#include <QVariantMap>

class Stateful : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QVariantMap properties READ properties NOTIFY propertiesChanged FINAL)

public:
	explicit Stateful(QObject *parent, std::shared_ptr<PBD::Stateful> stateful);

	QVariantMap properties();

Q_SIGNALS:
	/// Signal to QML that properties changed
	void propertiesChanged();

private Q_SLOTS:
	/// Called from PBD when one or more properties changed
	void updateProperties(const PBD::PropertyChange& change);

protected:
	std::shared_ptr<PBD::Stateful> m_stateful;

private:
	QVariantMap m_properties;
};

#endif // STATEFUL_H
