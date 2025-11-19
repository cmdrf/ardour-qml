#ifndef AUDIOREGION_H
#define AUDIOREGION_H

#include "Region.h"

#include <ardour/audioregion.h>

class AudioRegion : public Region
{
	Q_OBJECT

	Q_PROPERTY(bool envelopeActive READ envelopeActive WRITE setEnvelopeActive NOTIFY envelopeActiveChanged)
//	Q_PROPERTY(bool defaultFadeIn READ defaultFadeIn WRITE setDefaultFadeIn NOTIFY defaultFadeInChanged)
//	Q_PROPERTY(bool defaultFadeOut READ defaultFadeOut WRITE setDefaultFadeOut NOTIFY defaultFadeOutChanged)
	Q_PROPERTY(bool fadeInActive READ fadeInActive WRITE setFadeInActive NOTIFY fadeInActiveChanged)
	Q_PROPERTY(bool fadeOutActive READ fadeOutActive WRITE setFadeOutActive NOTIFY fadeOutActiveChanged)
	Q_PROPERTY(bool fadeBeforeFx READ fadeBeforeFx WRITE setFadeBeforeFx NOTIFY fadeBeforeFxChanged)
	Q_PROPERTY(float scaleAmplitude READ scaleAmplitude WRITE setScaleAmplitude NOTIFY scaleAmplitudeChanged)

	Q_DISABLE_COPY(AudioRegion)

public:
	explicit AudioRegion(QObject *parent, std::shared_ptr<ARDOUR::AudioRegion> audioRegion);
	~AudioRegion();

	std::shared_ptr<ARDOUR::AudioRegion> audioRegion() {return std::dynamic_pointer_cast<ARDOUR::AudioRegion>(m_stateful);}
	const std::shared_ptr<ARDOUR::AudioRegion> audioRegion() const {return std::dynamic_pointer_cast<ARDOUR::AudioRegion>(m_stateful);}

	bool envelopeActive() const {return audioRegion()->envelope_active();}
	void setEnvelopeActive(bool active) {audioRegion()->set_envelope_active(active);}
	bool fadeInActive() const {return audioRegion()->fade_in_active();}
	void setFadeInActive(bool active) {audioRegion()->set_fade_in_active(active);}
	bool fadeOutActive() const {return audioRegion()->fade_out_active();}
	void setFadeOutActive(bool active) {audioRegion()->set_fade_out_active(active);}
	bool fadeBeforeFx() const {return audioRegion()->fade_before_fx();}
	void setFadeBeforeFx(bool before) {audioRegion()->set_fade_before_fx(before);}
	float scaleAmplitude() const {return audioRegion()->scale_amplitude();}
	void setScaleAmplitude(float scale) {audioRegion()->set_scale_amplitude(scale);}

Q_SIGNALS:
	void envelopeActiveChanged();
	void fadeInActiveChanged();
	void fadeOutActiveChanged();
	void fadeBeforeFxChanged();
	void scaleAmplitudeChanged();

private Q_SLOTS:
	void updateProperties(const PBD::PropertyChange& change);

private:
};

#endif // AUDIOREGION_H
