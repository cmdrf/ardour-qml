#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <rhi/qrhi.h>
#include <QQuickRhiItemRenderer>

class WaveformRenderer : public QQuickRhiItemRenderer
{
public:
	void initialize(QRhiCommandBuffer *cb) override;
	void synchronize(QQuickRhiItem *item) override;
	void render(QRhiCommandBuffer *cb) override;

private:
	QRhi *m_rhi = nullptr;
	std::unique_ptr<QRhiBuffer> m_vbuf;
	std::unique_ptr<QRhiBuffer> m_ubuf;
	std::unique_ptr<QRhiShaderResourceBindings> m_srb;
	std::unique_ptr<QRhiGraphicsPipeline> m_pipeline;
	QMatrix4x4 m_viewProjection;
	float m_angle = 0.0f;
};

class Waveform : public QQuickRhiItem
{
	Q_OBJECT
	QML_NAMED_ELEMENT(Waveform)
	Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)

public:
	QQuickRhiItemRenderer *createRenderer() override;

	float angle() const { return m_angle; }
	void setAngle(float a);

Q_SIGNALS:
	void angleChanged();

private:
	float m_angle = 0.0f;
};

#endif // WAVEFORM_H
