#ifndef LOCATION_H
#define LOCATION_H

#include "StatefulDestructible.h"
#include "TimePos.h"

#include <ardour/location.h>

#include <QObject>
#include <QtQmlIntegration>

class Location : public StatefulDestructible
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_DISABLE_COPY(Location)

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(TimePos start READ start WRITE setStart NOTIFY startChanged FINAL)
    Q_PROPERTY(TimePos end READ end WRITE setEnd NOTIFY endChanged FINAL)
    Q_PROPERTY(bool locked READ locked WRITE setLocked NOTIFY lockedChanged FINAL)
    Q_PROPERTY(qint64 timestamp READ timestamp NOTIFY timestampChanged FINAL)
    Q_PROPERTY(int32_t cueId READ cueId WRITE setCueId NOTIFY cueIdChanged FINAL)

    /* Flag-derived boolean properties (all notify on flagsChanged internally) */
    Q_PROPERTY(bool mark READ isMark NOTIFY markChanged FINAL)
    Q_PROPERTY(bool autoPunch READ isAutoPunch WRITE setAutoPunch NOTIFY autoPunchChanged FINAL)
    Q_PROPERTY(bool autoLoop READ isAutoLoop WRITE setAutoLoop NOTIFY autoLoopChanged FINAL)
    Q_PROPERTY(bool hidden READ isHidden WRITE setHidden NOTIFY hiddenChanged FINAL)
    Q_PROPERTY(bool cdMarker READ isCDMarker WRITE setCD NOTIFY cdMarkerChanged FINAL)
    Q_PROPERTY(bool rangeMarker READ isRangeMarker WRITE setIsRangeMarker NOTIFY rangeMarkerChanged FINAL)
    Q_PROPERTY(bool skip READ isSkip WRITE setSkip NOTIFY skipChanged FINAL)
    Q_PROPERTY(bool skipping READ isSkipping WRITE setSkipping NOTIFY skippingChanged FINAL)
    Q_PROPERTY(bool clockOrigin READ isClockOrigin WRITE setIsClockOrigin NOTIFY clockOriginChanged FINAL)
    Q_PROPERTY(bool section READ isSection WRITE setSection NOTIFY sectionChanged FINAL)

public:
    Location(QObject* parent, std::shared_ptr<ARDOUR::Location> location);

    std::shared_ptr<ARDOUR::Location> location() { return std::dynamic_pointer_cast<ARDOUR::Location>(m_stateful); }
    const std::shared_ptr<ARDOUR::Location> location() const { return std::dynamic_pointer_cast<ARDOUR::Location>(m_stateful); }

    QString name() const { return QString::fromStdString(location()->name()); }
    void setName(const QString& n) { location()->set_name(n.toStdString()); }

    TimePos start() const { return location()->start(); }
    TimePos end() const { return location()->end(); }
    void setStart(const TimePos& s) { location()->set_start(s); }
    void setEnd(const TimePos& e) { location()->set_end(e); }

    bool locked() const { return location()->locked(); }
    void setLocked(bool yn) { if(yn) location()->lock(); else location()->unlock(); }

    qint64 timestamp() const { return location()->timestamp(); }

    int32_t cueId() const { return location()->cue_id(); }
    void setCueId(int32_t id) { location()->set_cue_id(id); }

    bool isAutoPunch() const { return location()->is_auto_punch(); }
    void setAutoPunch(bool yn) { location()->set_auto_punch(yn, nullptr); }

    bool isAutoLoop() const { return location()->is_auto_loop(); }
    void setAutoLoop(bool yn) { location()->set_auto_loop(yn, nullptr); }

    bool isMark() const { return location()->is_mark(); }
    bool isHidden() const { return location()->is_hidden(); }
    void setHidden(bool yn) { location()->set_hidden(yn, nullptr); }

    bool isCDMarker() const { return location()->is_cd_marker(); }
    void setCD(bool yn) { location()->set_cd(yn, nullptr); }

    bool isRangeMarker() const { return location()->is_range_marker(); }
    void setIsRangeMarker(bool yn) { location()->set_is_range_marker(yn, nullptr); }

    bool isSkip() const { return location()->is_skip(); }
    void setSkip(bool yn) { location()->set_skip(yn); }

    bool isSkipping() const { return location()->is_skipping(); }
    void setSkipping(bool yn) { location()->set_skipping(yn); }

    bool isClockOrigin() const { return location()->is_clock_origin(); }
    void setIsClockOrigin(bool yn) { location()->set_is_clock_origin(yn, nullptr); }

    bool isSection() const { return location()->is_section(); }
    void setSection(bool yn) { location()->set_section(yn); }

Q_SIGNALS:
    void nameChanged();
    void startChanged();
    void endChanged();
    void lockedChanged();
    void timestampChanged();
    void cueIdChanged();

    /* flag notifications */
    void markChanged();
    void autoPunchChanged();
    void autoLoopChanged();
    void hiddenChanged();
    void cdMarkerChanged();
    void rangeMarkerChanged();
    void skipChanged();
    void skippingChanged();
    void clockOriginChanged();
    void sectionChanged();

private Q_SLOTS:
	void onFlagsChanged();

private:
};

#endif // LOCATION_H
