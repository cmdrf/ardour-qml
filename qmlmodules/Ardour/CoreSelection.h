#ifndef CORESELECTION_H
#define CORESELECTION_H

#include "AutomationControl.h"
#include "Stripable.h"

#include <ardour/selection.h>

#include <QtQml>

/// Handles global selection of stripables
/** ARDOUR::CoreSelection derives from PBD::Stateful, but our Stateful's shared_ptr messes with
ownership. Therefore, we derive from QObject. */
class CoreSelection : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_UNCREATABLE("Access via Ardour.session.selection")

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

	CoreSelection(QObject* parent, ARDOUR::CoreSelection& coreSelection);

	Q_INVOKABLE bool selectStripableAndMaybeGroup(Stripable* s, SelectionOperation op, bool withGroup = true, bool routesOnly = true);
	Q_INVOKABLE void selectStripableWithControl(Stripable* s, AutomationControl* c, SelectionOperation op);

	/// Get first selected stripable
	/** @see firstSelectedStripableChanged */
	Stripable* firstSelectedStripable();

Q_SIGNALS:
	void firstSelectedStripableChanged();

public Q_SLOTS:
	void selectNextStripable (bool mixerOrder, bool routesOnly);
	void selectPrevStripable (bool mixerOrder, bool routesOnly);

private Q_SLOTS:
	void handlePresentationChange(const PBD::PropertyChange&);

private:
	ARDOUR::CoreSelection& m_coreSelection;
	QPointer<Stripable> m_firstSelectedStripable;
};

#endif // CORESELECTION_H
