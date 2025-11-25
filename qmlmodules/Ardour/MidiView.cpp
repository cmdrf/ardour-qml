#include "MidiView.h"

#include <ardour/midi_model.h>
#include <evoral/EventSink.h>

#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>

/*******************************************************/

MidiView::MidiView(QQuickItem *parent)
	: QQuickItem(parent)
{
	setFlag(ItemHasContents, true);
}

MidiView::~MidiView() = default;


QSGNode* MidiView::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
	QSGGeometryNode *node = nullptr;
	QSGGeometry *geometry = nullptr;

	if (!oldNode)
	{
		node = new QSGGeometryNode;

		geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), m_notes.size() * 6);
		geometry->setLineWidth(2);
		geometry->setDrawingMode(QSGGeometry::DrawTriangles);
		node->setGeometry(geometry);
		node->setFlag(QSGNode::OwnsGeometry);

		auto* material = new QSGFlatColorMaterial;
		material->setColor(QColor(255, 0, 0));
		node->setMaterial(material);
		node->setFlag(QSGNode::OwnsMaterial);
	}
	else
	{
		node = static_cast<QSGGeometryNode*>(oldNode);
		geometry = node->geometry();
		geometry->allocate(m_notes.size() * 6);
	}

	QSizeF itemSize = size();
	QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

	uint8_t maxPitch = 0;
	uint8_t minPitch = 255;
	for(auto& note: m_notes)
	{
		maxPitch = qMin(maxPitch, note.pitch);
		minPitch = qMax(minPitch, note.pitch);
	}
	const float pitchSpan = maxPitch - minPitch;

	for (int i = 0; i < m_notes.size(); ++i)
	{
		auto note = m_notes[i];
		float x0 = static_cast<float>(note.start) / m_regionLength * itemSize.width();
		float x1 = static_cast<float>(note.end) / m_regionLength * itemSize.width();
		float y0 = qCeil((note.pitch + 1 - minPitch) / pitchSpan * itemSize.height());
		float y1 = qFloor((note.pitch - minPitch) / pitchSpan * itemSize.height());

		vertices[i*6  ].set(x0, y0);
		vertices[i*6+1].set(x1, y0);
		vertices[i*6+2].set(x1, y1);

		vertices[i*6+3].set(x0, y0);
		vertices[i*6+4].set(x1, y1);
		vertices[i*6+5].set(x0, y1);
	}
	node->markDirty(QSGNode::DirtyGeometry);

	return node;
}

void MidiView::setMidiRegion(MidiRegion* region)
{
	if(region != m_midiRegion)
	{
		m_midiRegion = region;
		Q_EMIT midiRegionChanged();

		// Populate m_notes:
		m_notes.clear();
		if(m_midiRegion)
		{
			m_regionLength = m_midiRegion->region()->length_samples();
			auto seq = m_midiRegion->midiRegion()->model();

			// hold read lock while accessing
			auto lock = seq->read_lock();

			std::transform(seq->notes().begin(), seq->notes().end(), std::back_inserter(m_notes),
				[](const std::shared_ptr<Evoral::Note<Temporal::Beats>>& note) {
					ARDOUR::timepos_t startTime = ARDOUR::timepos_t (note->time());
					ARDOUR::samplepos_t startPos = startTime.samples();
					ARDOUR::timepos_t endTime = ARDOUR::timepos_t (note->end_time());
					ARDOUR::samplepos_t endPos = endTime.samples();

					return Note{
						startPos,
						endPos,
						note->note()
					};
				}
			);
		}
	}
}
