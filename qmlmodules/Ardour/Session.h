#ifndef SESSION_H
#define SESSION_H

#include "Enums.h"
#include "PluginInfo.h"
#include "Processor.h"
#include "RouteGroup.h"
#include "RouteModel.h"
#include "CoreSelection.h"
#include "TempoMap.h"
#include "Track.h"

#include <ardour/session.h>

#include <QObject>
#include <QtQmlIntegration>

namespace ARDOUR
{
class Session;
}

class ChanCount;
class Location;
class Track;

/// Wrapper around ARDOUR::Session
class Session : public QObject
{
    Q_OBJECT
	QML_ANONYMOUS
    Q_DISABLE_COPY(Session)

	Q_PROPERTY(bool dirty READ dirty NOTIFY dirtyChanged FINAL)
	Q_PROPERTY(RecordState recordState READ recordState NOTIFY recordStateChanged FINAL)

	/** 0 if stopped. */
	Q_PROPERTY(float transportSpeed READ transportSpeed NOTIFY transportSpeedChanged FINAL)

	Q_PROPERTY(bool playLoop READ playLoop NOTIFY playLoopChanged FINAL)

	Q_PROPERTY(qint64 transportSample READ transportSample NOTIFY transportSampleChanged FINAL)

	Q_PROPERTY(RouteModel* routes READ routes CONSTANT FINAL)
	Q_PROPERTY(QAbstractItemModel* tracks READ tracks CONSTANT FINAL)
	Q_PROPERTY(CoreSelection* selection READ selection CONSTANT FINAL)

	Q_PROPERTY(TimePos currentEnd READ currentEnd WRITE setCurrentEnd NOTIFY currentEndChanged FINAL)
	Q_PROPERTY(TimePos currentStart READ currentStart WRITE setCurrentStart NOTIFY currentStartChanged FINAL)

	Q_PROPERTY(TempoMap* tempoMap READ tempoMap CONSTANT FINAL)

	Q_PROPERTY(Location* autoLoopLocation READ autoLoopLocation NOTIFY autoLoopLocationChanged FINAL)
	Q_PROPERTY(Location* autoPunchLocation READ autoPunchLocation NOTIFY autoPunchLocationChanged FINAL)

public:
	enum RecordState
	{
		Disabled,
		Enabled,
		Recording
	};
	Q_ENUM(RecordState);

	Session(QObject* parent, ARDOUR::Session* session);

	ARDOUR::Session* session() {return m_session;}

	bool dirty() const;

	RecordState recordState() const;

	float transportSpeed() const {return m_transportSpeed;}
	bool playLoop() const {return m_playLoop;}
	qint64 transportSample() const {return m_session->transport_sample();}

	RouteModel* routes() {return &m_routes;}
	QAbstractItemModel* tracks() {return &m_tracks;}
	CoreSelection* selection() {return &m_selection;}
	TempoMap* tempoMap() {return &m_tempoMap;}

	Q_INVOKABLE Processor* newPlugin(const PluginInfo& info, const QString& preset);
	Q_INVOKABLE Track* newAudioTrack(
			int inputChannels,
			int outputChannels,
			RouteGroup* routeGroup = nullptr,
			int order = ARDOUR::PresentationInfo::max_order,
			Ardour::TrackMode mode = Ardour::NormalTrackMode);

	Q_INVOKABLE Track* newMidiTrack (
		const ChanCount& input, const ChanCount& output, bool strictIo,
		PluginInfo* instrument,
		void* preset = nullptr, // TODO
		RouteGroup* routeGroup = nullptr,
		int order = ARDOUR::PresentationInfo::max_order,
		Ardour::TrackMode mode = Ardour::NormalTrackMode,
		bool inputAutoConnect = true,
		bool triggerVisibility = false
		);

	TimePos currentEnd() const;
	void setCurrentEnd(const TimePos& newCurrentEnd);

	TimePos currentStart() const;
	void setCurrentStart(const TimePos& newCurrentStart);

	Location* autoLoopLocation();
	Q_INVOKABLE void setAutoLoopLocation(const TimePos& start, const TimePos& end);

	Location* autoPunchLocation();
	Q_INVOKABLE void setAutoPunchLocation(const TimePos& start, const TimePos& end);

public Q_SLOTS:
	void maybeEnableRecord();
	void disableRecord();
	void requestRoll(); // Play
	void requestLocate(qint64 sample);
	void requestStop();

Q_SIGNALS:
	void dirtyChanged();
	void recordStateChanged();
	void transportSpeedChanged();
	void playLoopChanged();
	void transportSampleChanged();
	void currentEndChanged();
	void currentStartChanged();
	void autoLoopLocationChanged();
	void autoPunchLocationChanged();

private Q_SLOTS:
	void transportStateChange();
	void onAutoLoopLocationChanged();
	void onAutoPunchLocationChanged();

private:
	ARDOUR::Session* m_session;
	RecordState m_recordState;
	float m_transportSpeed;
	bool m_playLoop;
	RouteModel m_routes;
	QSortFilterProxyModel m_tracks;
	CoreSelection m_selection;
	TempoMap m_tempoMap;

	/** Timer to regularly update the transport position during play. */
	QTimer m_transportPositionQueryTimer;

	Location* m_autoLoopLocation = nullptr;
	Location* m_autoPunchLocation = nullptr;
};

#endif // SESSION_H
