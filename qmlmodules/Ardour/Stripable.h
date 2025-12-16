#ifndef STRIPABLE_H
#define STRIPABLE_H

#include "SessionObject.h"

class Stripable : public SessionObject
{
	Q_OBJECT
public:
	explicit Stripable(QObject* parent, std::shared_ptr<ARDOUR::Stripable> stripable);

	std::shared_ptr<ARDOUR::Stripable> stripable() {return std::dynamic_pointer_cast<ARDOUR::Stripable>(m_stateful);}
	const std::shared_ptr<ARDOUR::Stripable> stripable() const {return std::dynamic_pointer_cast<ARDOUR::Stripable>(m_stateful);}

};

#endif // STRIPABLE_H
