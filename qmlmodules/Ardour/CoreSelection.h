#ifndef CORESELECTION_H
#define CORESELECTION_H

#include "AutomationControl.h"
#include "Stateful.h"
#include "Stripable.h"

#include <ardour/selection.h>

class CoreSelection : public Stateful
{
	Q_OBJECT

	Q_PROPERTY(Stripable* firstSelectedStripable READ firstSelectedStripable NOTIFY firstSelectedStripableChanged FINAL)

public:
	enum SelectionOperation
	{
		SelectionSet = ARDOUR::SelectionSet,
		SelectionAdd = ARDOUR::SelectionAdd,
		SelectionToggle = ARDOUR::SelectionToggle,
		SelectionRemove = ARDOUR::SelectionRemove,
		SelectionExtend = ARDOUR::SelectionExtend
	};
	Q_ENUM(SelectionOperation)

	CoreSelection(QObject* parent, std::shared_ptr<ARDOUR::CoreSelection> coreSelection);
	CoreSelection(QObject* parent, ARDOUR::CoreSelection& coreSelection);

	std::shared_ptr<ARDOUR::CoreSelection> coreSelection() {return std::dynamic_pointer_cast<ARDOUR::CoreSelection>(m_stateful);}
	const std::shared_ptr<ARDOUR::CoreSelection> coreSelection() const {return std::dynamic_pointer_cast<ARDOUR::CoreSelection>(m_stateful);}

	Q_INVOKABLE bool selectStripableAndMaybeGroup(Stripable* s, SelectionOperation op, bool withGroup = true, bool routesOnly = true);
	Q_INVOKABLE void selectStripableWithControl(Stripable* s, AutomationControl* c, SelectionOperation op);

	Stripable* firstSelectedStripable();

Q_SIGNALS:
	void firstSelectedStripableChanged();


public Q_SLOTS:
	void selectNextStripable (bool mixerOrder, bool routesOnly);
	void selectPrevStripable (bool mixerOrder, bool routesOnly);

private Q_SLOTS:
	void handlePresentationChange(const PBD::PropertyChange&);

private:
	QPointer<Stripable> m_firstSelectedStripable;
};

#endif // CORESELECTION_H
