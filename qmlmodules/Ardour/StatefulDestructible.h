#ifndef STATEFULDESTRUCTIBLE_H
#define STATEFULDESTRUCTIBLE_H

#include "QtBridgeUi.h"
#include "Stateful.h"

#include <pbd/statefuldestructible.h>

/// Can self-destruct
class StatefulDestructible : public Stateful
{
	Q_OBJECT

public:
	StatefulDestructible(QObject *parent, std::shared_ptr<PBD::StatefulDestructible> stateful) :
		Stateful(parent, stateful)
	{
		QtBridgeUi& b = QtBridgeUi::instance();

		b.connect(stateful->DropReferences, this, &QObject::deleteLater);
	}
};

#endif // STATEFULDESTRUCTIBLE_H
