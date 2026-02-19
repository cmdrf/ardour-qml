#ifndef TIMEPOS_H
#define TIMEPOS_H

#include "Beats.h"

#include <ardour/types.h>

#include <QObject>
#include <QtQmlIntegration>

/// Wrapper around ARDOUR::timepos_t
class TimePos
{
	Q_GADGET
	Q_PROPERTY(bool isBeats READ isBeats)
	Q_PROPERTY(qint64 samples READ samples)
	Q_PROPERTY(qint64 ticks READ ticks)

	QML_VALUE_TYPE(timepos)

public:
	TimePos() {}
	TimePos(ARDOUR::timepos_t t) : m_t(t) {}

	bool isBeats() const {return m_t.is_beats();}
	qint64 samples() const {return m_t.samples();}
	qint64 ticks() const {return m_t.ticks();}

	// Conversion operator to timepos_t:
	operator ARDOUR::timepos_t() const {return m_t;}

private:
	ARDOUR::timepos_t m_t;
};

/// Singleton containing functions for constructing TimePos
class TimePosFuncs : public QObject
{
	Q_OBJECT
	QML_NAMED_ELEMENT(TimePos)
	QML_SINGLETON

public:
	/// Construct TimePos from samples
	Q_INVOKABLE TimePos fromSamples(qint64 samples) {return ARDOUR::timepos_t(samples);}

	/// Construct TimePos from Beats
	Q_INVOKABLE TimePos fromBeats(const Beats& beats) {return ARDOUR::timepos_t(beats);}

	/// Construct TimePos from ticks
	Q_INVOKABLE TimePos fromTicks(qint64 ticks) {return ARDOUR::timepos_t::from_ticks(ticks);}
};

#endif // TIMEPOS_H
