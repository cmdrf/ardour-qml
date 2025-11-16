#ifndef TIMECOUNT_H
#define TIMECOUNT_H

#include "TimePos.h"
#include <ardour/types.h>

#include <QObject>
#include <QtQmlIntegration>

class TimeCount
{
	Q_GADGET
	Q_PROPERTY(qint64 magnitude READ magnitude)
	Q_PROPERTY(TimePos position READ position WRITE setPosition)
	Q_PROPERTY(TimePos end READ end)
	Q_PROPERTY(qint64 samples READ samples)
	Q_PROPERTY(qint64 ticks READ ticks)

	QML_VALUE_TYPE(timecnt)

public:
	TimeCount() {}
	TimeCount(ARDOUR::timecnt_t t) : m_t(t) {}

	qint64 magnitude() const {return m_t.magnitude();}
	TimePos position() const {return m_t.position();}
	TimePos end() const {return m_t.end();}
	qint64 samples() const {return m_t.samples();}
	qint64 ticks() const {return m_t.ticks();}

	// Conversion operator to timepos_t:
	operator ARDOUR::timecnt_t() const {return m_t;}

	void setPosition(TimePos pos) {m_t.set_position(pos);}

private:
	ARDOUR::timecnt_t m_t;
};

#endif // TIMECOUNT_H
