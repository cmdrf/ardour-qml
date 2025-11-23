#ifndef MIDIREGION_H
#define MIDIREGION_H

#include "Region.h"

#include <ardour/midi_region.h>

class MidiRegion : public Region
{
	Q_OBJECT
public:
	explicit MidiRegion(QObject* parent, std::shared_ptr<ARDOUR::MidiRegion> midiRegion);

	std::shared_ptr<ARDOUR::MidiRegion> midiRegion() {return std::dynamic_pointer_cast<ARDOUR::MidiRegion>(m_stateful);}
	const std::shared_ptr<ARDOUR::MidiRegion> midiRegion() const {return std::dynamic_pointer_cast<ARDOUR::MidiRegion>(m_stateful);}
};

#endif // MIDIREGION_H
