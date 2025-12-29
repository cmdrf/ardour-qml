#ifndef AUDIOTRIGGER_H
#define AUDIOTRIGGER_H

#include "Trigger.h"

#include <ardour/triggerbox.h>

class AudioTrigger : public Trigger
{
	Q_OBJECT

	Q_PROPERTY(StretchMode stretchMode READ stretchMode WRITE setStretchMode NOTIFY stretchModeChanged FINAL)

public:
	AudioTrigger(QObject *parent, std::shared_ptr<ARDOUR::AudioTrigger> audioTrigger);

	std::shared_ptr<ARDOUR::AudioTrigger> audioTrigger() {return std::dynamic_pointer_cast<ARDOUR::AudioTrigger>(m_stateful);}
	const std::shared_ptr<ARDOUR::AudioTrigger> audioTrigger() const {return std::dynamic_pointer_cast<ARDOUR::AudioTrigger>(m_stateful);}

	StretchMode stretchMode() const {return static_cast<StretchMode>(audioTrigger()->stretch_mode());}
	void setStretchMode(const StretchMode& newStretchMode) {audioTrigger()->set_stretch_mode(static_cast<ARDOUR::Trigger::StretchMode>(newStretchMode));}

Q_SIGNALS:
	void stretchModeChanged();

private Q_SLOTS:
	void updateProperties(const PBD::PropertyChange& change);

};

#endif // AUDIOTRIGGER_H
