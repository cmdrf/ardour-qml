#ifndef BEATS_H
#define BEATS_H

#include <temporal/beats.h>

#include <QObject>
#include <QtQmlIntegration>

/// Wrapper around Temporal::Beats to make it available in QML.
class Beats
{
	Q_GADGET
	QML_VALUE_TYPE(beats)

	/// Beats
	Q_PROPERTY(qint64 beats READ beats FINAL)

	/// Ticks
	Q_PROPERTY(qint32 ticks READ ticks FINAL)

public:
	Beats() = default;
	Beats(Temporal::Beats t) : m_b(t) {}

	qint64 beats() const {return m_b.get_beats();}
	qint32 ticks() const {return m_b.get_ticks();}

	Q_INVOKABLE Beats roundToBeat() const {return m_b.round_to_beat();}
	Q_INVOKABLE Beats roundUpToBeat() const {return m_b.round_up_to_beat();}
	Q_INVOKABLE Beats roundDownToBeat() const {return m_b.round_down_to_beat();}

	/// Conversion operator to Temporal::Beats:
	operator Temporal::Beats() const {return m_b;}

private:
	Temporal::Beats m_b;
};
#endif // BEATS_H
