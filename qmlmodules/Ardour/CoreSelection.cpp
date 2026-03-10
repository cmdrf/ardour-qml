#include "CoreSelection.h"

#include <ardour/selection.h>
#include <ardour/stripable.h>


CoreSelection::CoreSelection(QObject* parent, ARDOUR::CoreSelection& coreSelection) :
	m_coreSelection(coreSelection)
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(ARDOUR::PresentationInfo::Change, this, &CoreSelection::handlePresentationChange);
}

bool CoreSelection::selectStripableAndMaybeGroup(Stripable* s, SelectionOperation op, bool withGroup, bool routesOnly)
{
	return m_coreSelection.select_stripable_and_maybe_group(s->stripable(), static_cast<ARDOUR::SelectionOperation>(op), withGroup, routesOnly);
}

void CoreSelection::selectStripableWithControl(Stripable* s, AutomationControl* c, SelectionOperation op)
{
	m_coreSelection.select_stripable_with_control(s->stripable(), c->automationControl(), static_cast<ARDOUR::SelectionOperation>(op));
}

Stripable* CoreSelection::firstSelectedStripable()
{
	return m_firstSelectedStripable;
}

void CoreSelection::selectNextStripable(bool mixerOrder, bool routesOnly)
{
	m_coreSelection.select_next_stripable(mixerOrder, routesOnly);
}

void CoreSelection::selectPrevStripable(bool mixerOrder, bool routesOnly)
{
	m_coreSelection.select_prev_stripable(mixerOrder, routesOnly);
}

void CoreSelection::handlePresentationChange(PBD::PropertyChange const & change)
{
	if(change.contains(ARDOUR::Properties::selected))
	{
		auto fss = m_coreSelection.first_selected_stripable();
		if(!fss && m_firstSelectedStripable)
		{
			m_firstSelectedStripable->deleteLater();
			m_firstSelectedStripable.clear();
			Q_EMIT firstSelectedStripableChanged();
		}
		else if(fss && (!m_firstSelectedStripable || fss != m_firstSelectedStripable->stripable()))
		{
			if(m_firstSelectedStripable)
				m_firstSelectedStripable->deleteLater();
			m_firstSelectedStripable = Stripable::create(this, fss);

			// Signal if stripable self-destructs:
			connect(m_firstSelectedStripable, &QObject::destroyed, this, &CoreSelection::firstSelectedStripableChanged);

			Q_EMIT firstSelectedStripableChanged();
		}
	}
}
