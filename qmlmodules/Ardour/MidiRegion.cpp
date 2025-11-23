#include "MidiRegion.h"

MidiRegion::MidiRegion(QObject *parent, std::shared_ptr<ARDOUR::MidiRegion> midiRegion) :
	Region{parent, midiRegion}
{

}
