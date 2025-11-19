#include "AudioRegion.h"

AudioRegion::AudioRegion(QObject *parent, std::shared_ptr<ARDOUR::AudioRegion> audioRegion) :
	Region{parent, audioRegion}
{

}

void AudioRegion::updateProperties(const PBD::PropertyChange& change)
{
	for(auto id: change)
	{
		if(id == ARDOUR::Properties::envelope_active.property_id)
			Q_EMIT envelopeActiveChanged();
		else if(id == ARDOUR::Properties::fade_in_active.property_id)
			Q_EMIT fadeInActiveChanged();
		else if(id == ARDOUR::Properties::fade_out_active.property_id)
			Q_EMIT fadeOutActiveChanged();
		else if(id == ARDOUR::Properties::fade_before_fx.property_id)
			Q_EMIT fadeBeforeFxChanged();
		else if(id == ARDOUR::Properties::scale_amplitude.property_id)
			Q_EMIT scaleAmplitudeChanged();
	}
}

