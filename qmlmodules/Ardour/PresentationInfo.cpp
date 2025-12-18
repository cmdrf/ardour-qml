#include "PresentationInfo.h"
#include "QtBridgeUi.h"


PresentationInfo::PresentationInfo(QObject* parent, ARDOUR::PresentationInfo& presentationInfo) :
	m_presentationInfo(presentationInfo),
	m_order(presentationInfo.order()),
	m_color(presentationInfo.color())
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(presentationInfo.PropertyChanged, this, &PresentationInfo::handlePropertyChange);

}



QColor PresentationInfo::color() const
{
	return m_color;
}

void PresentationInfo::setColor(const QColor& newColor)
{
	m_presentationInfo.set_color(newColor.rgba());
}

void PresentationInfo::handlePropertyChange()
{
	if(m_order != m_presentationInfo.order())
	{
		m_order = m_presentationInfo.order();
		Q_EMIT orderChanged();
	}

	if(m_color != m_presentationInfo.color())
	{
		m_color = m_presentationInfo.color();
		Q_EMIT colorChanged();
	}

	if(m_hidden != m_presentationInfo.hidden())
	{
		m_hidden = m_presentationInfo.hidden();
		Q_EMIT hiddenChanged();
	}

	if(m_triggerTrack != m_presentationInfo.trigger_track())
	{
		m_triggerTrack = m_presentationInfo.trigger_track();
		Q_EMIT triggerTrackChanged();
	}
}

bool PresentationInfo::hidden() const
{
	return m_hidden;
}

void PresentationInfo::setHidden(bool newHidden)
{
	m_presentationInfo.set_hidden(newHidden);
}

void PresentationInfo::setTriggerTrack(bool newTriggerTrack)
{
	m_presentationInfo.set_trigger_track(newTriggerTrack);
}
