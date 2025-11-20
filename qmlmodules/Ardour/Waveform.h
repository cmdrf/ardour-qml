#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <ardour/types.h>

#include <rhi/qrhi.h>
#include <QQuickRhiItemRenderer>

class AudioRegion;

class Waveform : public QQuickRhiItem
{
	Q_OBJECT
	QML_NAMED_ELEMENT(Waveform)
	Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)
	Q_PROPERTY(AudioRegion* audioRegion READ audioRegion WRITE setAudioRegion NOTIFY audioRegionChanged FINAL)

	friend class WaveformRenderer;
public:
	QQuickRhiItemRenderer *createRenderer() override;

	float angle() const { return m_angle; }
	void setAngle(float a);

	AudioRegion* audioRegion() const {return m_region;}
	void setAudioRegion(AudioRegion* newRegion);

Q_SIGNALS:
	void angleChanged();
	void audioRegionChanged();

private:
	float m_angle = 0.0f;
	AudioRegion* m_region = nullptr;
	QList<ARDOUR::PeakData> m_peaksUpdate;
};

#endif // WAVEFORM_H
