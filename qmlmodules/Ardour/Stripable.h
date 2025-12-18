#ifndef STRIPABLE_H
#define STRIPABLE_H

#include "SessionObject.h"
#include "Controllable.h"

class Stripable : public SessionObject
{
	Q_OBJECT

	Q_PROPERTY(Controllable* soloControl READ soloControl CONSTANT)
	Q_PROPERTY(Controllable* muteControl READ muteControl CONSTANT)

public:
	explicit Stripable(QObject* parent, std::shared_ptr<ARDOUR::Stripable> stripable);

	std::shared_ptr<ARDOUR::Stripable> stripable() {return std::dynamic_pointer_cast<ARDOUR::Stripable>(m_stateful);}
	const std::shared_ptr<ARDOUR::Stripable> stripable() const {return std::dynamic_pointer_cast<ARDOUR::Stripable>(m_stateful);}

	Controllable* soloControl();
	Controllable* muteControl();

private:
	QPointer<Controllable> m_soloControl;
	QPointer<Controllable> m_muteControl;

};

#endif // STRIPABLE_H
