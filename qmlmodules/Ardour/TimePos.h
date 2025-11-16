#ifndef TIMEPOS_H
#define TIMEPOS_H

#include <ardour/types.h>

#include <QObject>
#include <QtQmlIntegration>

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

#endif // TIMEPOS_H
