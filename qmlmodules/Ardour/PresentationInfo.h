#ifndef PRESENTATIONINFO_H
#define PRESENTATIONINFO_H

#include <ardour/presentation_info.h>

#include <QColor>
#include <QObject>

/** Normally this would derive from Stateful, but our Stateful stores a std::shared_ptr, but
ARDOUR::Stripable stores a flat object. */
class PresentationInfo : public QObject // public Stateful
{
	Q_OBJECT

	Q_PROPERTY(int order READ order NOTIFY orderChanged FINAL)
	Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
	Q_PROPERTY(bool hidden READ hidden WRITE setHidden NOTIFY hiddenChanged FINAL)
	Q_PROPERTY(bool triggerTrack READ triggerTrack WRITE setTriggerTrack NOTIFY triggerTrackChanged FINAL)

public:
	explicit PresentationInfo(QObject* parent, ARDOUR::PresentationInfo& presentationInfo);

	int order() const {return m_order;}

	QColor color() const;
	void setColor(const QColor& newColor);

	bool hidden() const;
	void setHidden(bool newHidden);

	bool triggerTrack() const {return m_triggerTrack;}
	void setTriggerTrack(bool newTriggerTrack);

Q_SIGNALS:
	void orderChanged();
	void colorChanged();
	void hiddenChanged();
	void triggerTrackChanged();

private Q_SLOTS:
	void handlePropertyChange();

private:
	ARDOUR::PresentationInfo& m_presentationInfo;

	int m_order;
	QColor m_color;
	bool m_hidden;
	bool m_triggerTrack;
};

#endif // PRESENTATIONINFO_H
