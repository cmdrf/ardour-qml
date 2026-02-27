#ifndef TEMPOMAP_H
#define TEMPOMAP_H

#include "Beats.h"
#include "TimeCount.h"
#include "TimePos.h"

#include <temporal/tempo.h>
#include <ardour/session.h>

#include <QAbstractListModel>

class Tempo {
	Q_GADGET

	Q_PROPERTY(double noteTypesPerMinute READ noteTypesPerMinute WRITE setNoteTypesPerMinute FINAL)
	Q_PROPERTY(int noteType READ noteType NOTIFY noteTypeChanged FINAL)
	Q_PROPERTY(double quarterNotesPerMinute READ quarterNotesPerMinute NOTIFY quarterNotesPerMinuteChanged FINAL)

public:
	Tempo() : m_tempo(120, 4) {}
	Tempo(const Temporal::Tempo& t) : m_tempo(t) {}

	double noteTypesPerMinute() const {return m_tempo.note_types_per_minute();}
	void setNoteTypesPerMinute(double npm) {m_tempo.set_note_types_per_minute(npm);}
	int noteType() const {return m_tempo.note_type();}
	double quarterNotesPerMinute() const {return m_tempo.quarter_notes_per_minute();}

	/// Conversion operator to Temporal::Tempo
	operator Temporal::Tempo() const {return m_tempo;}

private:
	Temporal::Tempo m_tempo;
};

class TempoFuncs : public QObject
{
	Q_OBJECT
	QML_NAMED_ELEMENT(Tempo)
	QML_SINGLETON

public:
	/// Construct Tempo
	Q_INVOKABLE Tempo tempo(double npm, int8_t noteType = 4) {return Temporal::Tempo(npm, noteType);}
};

class Meter {
	Q_GADGET

	Q_PROPERTY(int divisionsPerBar READ divisionsPerBar FINAL)
	Q_PROPERTY(int noteValue READ noteValue FINAL)

public:
	Meter(const Temporal::Meter& m) : m_meter(m) {}

	int divisionsPerBar() const {return m_meter.divisions_per_bar();}
	int noteValue() const {return m_meter.note_value();}

	/// Conversion operator to Temporal::Meter
	operator Temporal::Meter() const {return m_meter;}

private:
	Temporal::Meter m_meter;
};

class MeterFuncs : public QObject
{
	Q_OBJECT
	QML_NAMED_ELEMENT(Meter)
	QML_SINGLETON

public:
	/// Construct Meter
	Q_INVOKABLE Meter meter(int divisionsPerBar, int noteValue) {return Temporal::Meter(divisionsPerBar, noteValue);}
};

class BbtArgument
{
	Q_GADGET

	Q_PROPERTY(int bars READ bars WRITE setBars FINAL)
	Q_PROPERTY(int beats READ beats WRITE setBeats FINAL)
	Q_PROPERTY(int ticks READ ticks WRITE setTicks FINAL)

public:
	BbtArgument() = default;
	BbtArgument(const Temporal::BBT_Argument& bbt) : m_bbt(bbt) {}

	int bars() const {return m_bbt.bars;}
	void setBars(int newBars) {m_bbt.bars = newBars;}

	int beats() const {return m_bbt.beats;}
	void setBeats(int newBeats) {m_bbt.beats = newBeats;}

	int ticks() const {return m_bbt.ticks;}
	void setTicks(int newTicks) {m_bbt.ticks = newTicks;}

	/// Conversion operator to Temporal::Tempo
	operator Temporal::BBT_Argument() const {return m_bbt;}

private:
	Temporal::BBT_Argument m_bbt;
};

class TempoMap : public QAbstractListModel
{
	Q_OBJECT
	QML_ELEMENT
	QML_UNCREATABLE("")

public:
	enum Type
	{
		MusicTimePointType,
		TempoPointType,
		MeterPointType
	};
	Q_ENUM(Type)

	enum
	{
		TimeRole = Qt::UserRole,
		TypeRole,
		NoteValueRole,
		DivisionsPerBarRole,
		NoteTypesPerMinuteRole
	};

	explicit TempoMap(QObject *parent, ARDOUR::Session* session);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

	Q_INVOKABLE BbtArgument bbtAt(const TimePos& time) const;
	Q_INVOKABLE Beats quartersAt(const TimePos& time) const;
	Q_INVOKABLE qint64 sampleAt(const Beats& beats) const;
	Q_INVOKABLE qint64 sampleAt(const TimePos& time) const;

	Q_INVOKABLE void removeTime(const TimePos& pos, const TimeCount& duration);

	/// Add new or overwrite existing tempo change at timePos
	/** Example:
	@code
	MouseArea {
		function roundBeats(samples) {
			let beats = TempoMap.quartersAt(TimePos.fromSamples(samples));
			let roundedBeats = beats.roundToBeat();
			return TempoMap.sampleAt(roundedBeats);
		}

		onClicked: (mouse) => {
			TempoMap.setTempo(TempoFuncs.tempo(Math.random() * 100 + 100, 4), TimePos.fromSamples(roundBeats(mouse.x * samplesPerPixel)));
		}
	}
	@endcode
	*/
	Q_INVOKABLE void setTempo(const Tempo& tempo, const TimePos& timePos);

	/// Add new or overwrite existing meter change at timePos
	Q_INVOKABLE void setMeter(const Meter& meter, const TimePos& timePos);

private Q_SLOTS:
	void updateMap();

private:
	struct Entry
	{
		TimePos time;
		Type type;
		int noteValue;
		int divisionsPerBar;
		double noteTypesPerMinute;
	};
	QVector<Entry> m_entries;
	ARDOUR::Session* m_session = nullptr;
};

#endif // TEMPOMAP_H
