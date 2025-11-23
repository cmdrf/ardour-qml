#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <ardour/types.h>

#include <rhi/qrhi.h>
#include <QQuickRhiItemRenderer>
#include <QFutureWatcher>

class AudioRegion;

class Waveform : public QQuickRhiItem
{
	Q_OBJECT
	QML_NAMED_ELEMENT(Waveform)
	Q_PROPERTY(AudioRegion* audioRegion READ audioRegion WRITE setAudioRegion NOTIFY audioRegionChanged FINAL)

	friend class WaveformRenderer;
public:
	Waveform();
	QQuickRhiItemRenderer *createRenderer() override;

	AudioRegion* audioRegion() const {return m_region;}
	void setAudioRegion(AudioRegion* newRegion);

Q_SIGNALS:
	void angleChanged();
	void audioRegionChanged();

private Q_SLOTS:
	void updatePeaks();

private:
	AudioRegion* m_region = nullptr;
	QList<ARDOUR::PeakData> m_peaksUpdate;
	QFutureWatcher<QList<ARDOUR::PeakData>> m_peaksWatcher;
};

#endif // WAVEFORM_H
