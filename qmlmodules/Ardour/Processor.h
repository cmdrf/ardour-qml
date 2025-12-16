#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "SessionObject.h"

#include <ardour/processor.h>

class Processor : public SessionObject
{
	Q_OBJECT

	Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
	Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged FINAL)
	Q_PROPERTY(bool bypassable READ bypassable NOTIFY bypassableChanged FINAL)

public:
	explicit Processor(QObject *parent, std::shared_ptr<ARDOUR::Processor> processor);
	bool enabled() const;
	void setEnabled(bool newEnabled);

	bool active() const;
	void setActive(bool newActive);
	bool bypassable() const;

	std::shared_ptr<ARDOUR::Processor> processor() {return std::dynamic_pointer_cast<ARDOUR::Processor>(m_stateful);}
	const std::shared_ptr<ARDOUR::Processor> processor() const {return std::dynamic_pointer_cast<ARDOUR::Processor>(m_stateful);}

Q_SIGNALS:
	void enabledChanged();
	void activeChanged();
	void bypassableChanged();

private:

};

#endif // PROCESSOR_H
