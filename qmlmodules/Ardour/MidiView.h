#ifndef MIDIVIEW_H
#define MIDIVIEW_H

#include "MidiRegion.h"

#include <QtQuick/QQuickItem>

class MidiView : public QQuickItem
{
	Q_OBJECT

	Q_PROPERTY(MidiRegion* midiRegion READ midiRegion WRITE setMidiRegion NOTIFY midiRegionChanged)

	QML_ELEMENT

public:
	MidiView(QQuickItem *parent = nullptr);
	~MidiView();

	QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

	MidiRegion* midiRegion() const { return m_midiRegion; }

	void setMidiRegion(MidiRegion* region);

Q_SIGNALS:
	void midiRegionChanged();

private:
	struct Note
	{
		ARDOUR::samplepos_t start;
		ARDOUR::samplepos_t end;
		uint8_t pitch;
	};

	MidiRegion* m_midiRegion = nullptr;
	ARDOUR::samplecnt_t m_regionLength; ///< Copy of region()->length()
	QVector<Note> m_notes;
};

#endif // MIDIVIEW_H
