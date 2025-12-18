#ifndef CONTROLLABLE_H
#define CONTROLLABLE_H

#include "StatefulDestructible.h"
#include <pbd/controllable.h>
#include <QObject>

class Controllable : public StatefulDestructible
{
	Q_OBJECT
	Q_DISABLE_COPY(Controllable)

	Q_PROPERTY(QString name READ name CONSTANT)
	Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged FINAL)

public:
	explicit Controllable(QObject *parent, std::shared_ptr<PBD::Controllable> controllable);

	std::shared_ptr<PBD::Controllable> controllable() {return std::dynamic_pointer_cast<PBD::Controllable>(m_stateful);}
	const std::shared_ptr<PBD::Controllable> controllable() const {return std::dynamic_pointer_cast<PBD::Controllable>(m_stateful);}

	QString name() const {return QString::fromStdString(controllable()->name());}

	double value() const;
	void setValue(double newValue);

Q_SIGNALS:

	void valueChanged();

private:
};

#endif // CONTROLLABLE_H
