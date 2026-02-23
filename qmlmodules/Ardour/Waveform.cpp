#include "Waveform.h"
#include "AudioRegion.h"
#include "ardour/types.h"

#include <QFile>
#include <QtConcurrentRun>

class WaveformRenderer : public QQuickRhiItemRenderer
{
public:
	void initialize(QRhiCommandBuffer *cb) override;
	void synchronize(QQuickRhiItem *item) override;
	void render(QRhiCommandBuffer *cb) override;

private:
	void createPipeline(QRhiCommandBuffer* cb, QRhiResourceUpdateBatch *resourceUpdates);

	QRhi *m_rhi = nullptr;
	std::unique_ptr<QRhiBuffer> m_vbuf;
	std::unique_ptr<QRhiBuffer> m_ubuf;
	std::unique_ptr<QRhiTexture> m_peakMinTexture;
	std::unique_ptr<QRhiTexture> m_peakMaxTexture;
	std::unique_ptr<QRhiSampler> m_peakMaxSampler;
	std::unique_ptr<QRhiShaderResourceBindings> m_srb;
	std::unique_ptr<QRhiGraphicsPipeline> m_pipeline;
	QList<ARDOUR::PeakData> m_peaksUpdate;
};

static float vertexData[] = {
	-1.0f,   1.0f,
	-1.0f,  -1.0f,
	 1.0f,  -1.0f,
	 1.0f,  -1.0f,
	 1.0f,   1.0f,
	-1.0f,   1.0f,
};


static QShader getShader(const QString& name)
{
	QFile f(name);
	return f.open(QIODevice::ReadOnly) ? QShader::fromSerialized(f.readAll()) : QShader();
}

void WaveformRenderer::synchronize(QQuickRhiItem* rhiItem)
{
	Waveform *item = static_cast<Waveform *>(rhiItem);
	m_peaksUpdate.clear();
	qSwap(m_peaksUpdate, item->m_peaksUpdate);
}

void WaveformRenderer::createPipeline(QRhiCommandBuffer* cb, QRhiResourceUpdateBatch *resourceUpdates)
{
	m_vbuf.reset(m_rhi->newBuffer(QRhiBuffer::Immutable, QRhiBuffer::VertexBuffer, sizeof(vertexData)));
	m_vbuf->create();

	m_ubuf.reset(m_rhi->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, 64));
	m_ubuf->create();

	m_peakMinTexture.reset(m_rhi->newTexture(QRhiTexture::R32F, QSize(1, 1)));
	m_peakMinTexture->create();
	m_peakMaxTexture.reset(m_rhi->newTexture(QRhiTexture::R32F, QSize(1, 1)));
	m_peakMaxTexture->create();

	m_peakMaxSampler.reset(m_rhi->newSampler(QRhiSampler::Linear, QRhiSampler::Linear, QRhiSampler::None,
											 QRhiSampler::ClampToEdge, QRhiSampler::ClampToEdge));
	m_peakMaxSampler->create();

	m_srb.reset(m_rhi->newShaderResourceBindings());
	m_srb->setBindings({
		QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage, m_ubuf.get()),
		QRhiShaderResourceBinding::sampledTexture(1, QRhiShaderResourceBinding::FragmentStage, m_peakMinTexture.get(), m_peakMaxSampler.get()),
		QRhiShaderResourceBinding::sampledTexture(2, QRhiShaderResourceBinding::FragmentStage, m_peakMaxTexture.get(), m_peakMaxSampler.get())
	});
	m_srb->create();

	m_pipeline.reset(m_rhi->newGraphicsPipeline());
	m_pipeline->setShaderStages({
		{ QRhiShaderStage::Vertex, getShader(QLatin1String(":/shaders/color.vert.qsb")) },
		{ QRhiShaderStage::Fragment, getShader(QLatin1String(":/shaders/color.frag.qsb")) }
	});
	QRhiVertexInputLayout inputLayout;
	inputLayout.setBindings({
		{ 2 * sizeof(float) }
	});
	inputLayout.setAttributes({
		{ 0, 0, QRhiVertexInputAttribute::Float2, 0 }
	});
	m_pipeline->setVertexInputLayout(inputLayout);
	m_pipeline->setShaderResourceBindings(m_srb.get());
	m_pipeline->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
	m_pipeline->create();

	resourceUpdates->uploadStaticBuffer(m_vbuf.get(), vertexData);
	QImage dummyImage(1, 1, QImage::Format_RGBA8888);
	dummyImage.fill(Qt::transparent);
	resourceUpdates->uploadTexture(m_peakMaxTexture.get(), dummyImage);
	cb->resourceUpdate(resourceUpdates);
}

void WaveformRenderer::initialize(QRhiCommandBuffer* cb)
{
	if (m_rhi != rhi())
	{
		m_pipeline.reset();
		m_rhi = rhi();
	}

	if (!m_pipeline)
	{
		QRhiResourceUpdateBatch *resourceUpdates = m_rhi->nextResourceUpdateBatch();
		createPipeline(cb, resourceUpdates);
	}
}

void WaveformRenderer::render(QRhiCommandBuffer* cb)
{
	QRhiResourceUpdateBatch *resourceUpdates = m_rhi->nextResourceUpdateBatch();

	if(!m_peaksUpdate.isEmpty())
	{
		QSize newSize(1, m_peaksUpdate.size());
		if(newSize != m_peakMaxTexture->pixelSize())
		{
			m_peakMinTexture->setPixelSize(QSize(1, m_peaksUpdate.size()));
			m_peakMinTexture->create();
			m_peakMaxTexture->setPixelSize(QSize(1, m_peaksUpdate.size()));
			m_peakMaxTexture->create();
		}

		// Test:
/*		float a = 0.0;
		for(auto& b : m_peaksUpdate)
		{
			b.min = a;
			b.max = 1.0 - a;
			a += 1.0 / m_peaksUpdate.size();
		}
*/
		{
			// Use stride to only update "min" members:
			QRhiTextureSubresourceUploadDescription subUploadDesc(&m_peaksUpdate.front().min, m_peaksUpdate.size() * sizeof(ARDOUR::PeakData));
			subUploadDesc.setDataStride(sizeof(ARDOUR::PeakData));

			QRhiTextureUploadEntry uploadEntry(0, 0, subUploadDesc);
			QRhiTextureUploadDescription uploadDesc(uploadEntry);

			resourceUpdates->uploadTexture(m_peakMinTexture.get(), uploadDesc);
		}

		{
			// Calculate remaining size when starting from "max" member:
			const quint32 size = m_peaksUpdate.size() * sizeof(ARDOUR::PeakData) - sizeof(ARDOUR::PeakData::PeakDatum);
			// Upload buffer starting from "max" member, with stride:
			QRhiTextureSubresourceUploadDescription subUploadDesc(&m_peaksUpdate.front().max, size);
			subUploadDesc.setDataStride(sizeof(ARDOUR::PeakData));

			QRhiTextureUploadEntry uploadEntry(0, 0, subUploadDesc);
			QRhiTextureUploadDescription uploadDesc(uploadEntry);

			resourceUpdates->uploadTexture(m_peakMaxTexture.get(), uploadDesc);
		}
		m_peaksUpdate.clear();
	}

	const QColor clearColor = QColor::fromRgbF(0.4f, 0.7f, 0.0f, 1.0f);
	cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, resourceUpdates);

	cb->setGraphicsPipeline(m_pipeline.get());
	const QSize outputSize = renderTarget()->pixelSize();
	cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));
	cb->setShaderResources();
	const QRhiCommandBuffer::VertexInput vbufBinding(m_vbuf.get(), 0);
	cb->setVertexInput(0, 1, &vbufBinding);
	cb->draw(6);

	cb->endPass();
}

/*********************************************************************/

Waveform::Waveform()
{
	connect(&m_peaksWatcher, &QFutureWatcher<QList<ARDOUR::PeakData>>::finished, this, &Waveform::updatePeaks);
}


QQuickRhiItemRenderer* Waveform::createRenderer()
{
	return new WaveformRenderer;
}

void Waveform::setAudioRegion(AudioRegion* newRegion)
{
	if (m_region == newRegion)
		return;
	m_region = newRegion;
	m_peaksUpdate.clear();

	if(m_region)
	{
		const ARDOUR::samplecnt_t start = newRegion->audioRegion()->start_sample();
		const ARDOUR::samplecnt_t length = newRegion->audioRegion()->length_samples();
		QFuture<QVector<ARDOUR::PeakData>> future = QtConcurrent::run([=]() {
			// Code in this block will run in another thread
			QList<ARDOUR::PeakData> out(1024);
			newRegion->audioRegion()->read_peaks(out.data(), out.length(), start, length, 0, 64);
			return out;
		});
		m_peaksWatcher.setFuture(future);
	}
	Q_EMIT audioRegionChanged();
}

void Waveform::updatePeaks()
{
	m_peaksUpdate = m_peaksWatcher.result();
	update();
}
