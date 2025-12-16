#include "Stripable.h"

#include <ardour/stripable.h>

Stripable::Stripable(QObject *parent, std::shared_ptr<ARDOUR::Stripable> stripable)
	: SessionObject{parent, stripable}
{

}
