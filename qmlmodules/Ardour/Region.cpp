#include "Region.h"
#include "QtBridgeUi.h"


Region::Region(QObject *parent, std::shared_ptr<ARDOUR::Region> region)
	: Stateful{parent, region}
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(region->PropertyChanged, this, &Region::updateProperties);
}

Region::DataType Region::dataType() const
{
	ARDOUR::DataType type = region()->data_type();
	if(type == ARDOUR::DataType::AUDIO)
		return AudioType;
	else if(type == ARDOUR::DataType::MIDI)
		return MidiType;
	else
		return UnknownType;
}

void Region::setAncestralData(const TimePos& start, const TimeCount& length, float stretch, float shift)
{
	region()->set_ancestral_data(start, length, stretch, shift);
}

void Region::updateProperties(const PBD::PropertyChange& change)
{
	for(auto id: change)
	{
		if(id == ARDOUR::Properties::length.property_id)
		{
			Q_EMIT positionChanged();
			Q_EMIT lengthChanged();
		}
		else if(id == ARDOUR::Properties::start.property_id)
			Q_EMIT startChanged();
		else if(id == ARDOUR::Properties::sync_position.property_id)
			Q_EMIT syncPositionChanged();
		else if(id == ARDOUR::Properties::ancestral_start.property_id)
			Q_EMIT ancestralStartChanged();
		else if(id == ARDOUR::Properties::ancestral_length.property_id)
			Q_EMIT ancestralLengthChanged();
		else if(id == ARDOUR::Properties::hidden.property_id)
			Q_EMIT hiddenChanged();
		else if(id == ARDOUR::Properties::muted.property_id)
			Q_EMIT mutedChanged();
		else if(id == ARDOUR::Properties::whole_file.property_id)
			Q_EMIT wholeFileChanged();
		else if(id == ARDOUR::Properties::automatic.property_id)
			Q_EMIT automaticChanged();
		else if(id == ARDOUR::Properties::opaque.property_id)
			Q_EMIT opaqueChanged();
		else if(id == ARDOUR::Properties::locked.property_id)
			Q_EMIT lockedChanged();
		else if(id == ARDOUR::Properties::video_locked.property_id)
			Q_EMIT videoLockedChanged();
		else if(id == ARDOUR::Properties::position_locked.property_id)
			Q_EMIT positionLockedChanged();
		else if(id == ARDOUR::Properties::layering_index.property_id)
			Q_EMIT layeringIndexChanged();
		else if(id == ARDOUR::Properties::tags.property_id)
			Q_EMIT tagsChanged();
		else if(id == ARDOUR::Properties::reg_group.property_id)
			Q_EMIT regionGroupChanged();
		else if(id == ARDOUR::Properties::stretch.property_id)
			Q_EMIT stretchChanged();
		else if(id == ARDOUR::Properties::shift.property_id)
			Q_EMIT shiftChanged();
		else if(id == ARDOUR::Properties::contents.property_id)
			Q_EMIT contentsChanged();
		else if(id == ARDOUR::Properties::region_fx.property_id)
			Q_EMIT regionFxChanged();
		else if(id == ARDOUR::Properties::region_tempo.property_id)
			Q_EMIT regionTempoChanged();
		else if(id == ARDOUR::Properties::region_meter.property_id)
			Q_EMIT regionMeterChanged();

	}
}


