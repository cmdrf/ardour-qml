#include "QtBridgeUi.h"

#include <memory>

#include <QDebug>
#include <QMetaMethod>
#include <QPointer>

#include "pbd/abstract_ui.inc.cc" /* instantiate the template */

QtBridgeUi::QtBridgeUi(QObject *parent) :
	QObject{parent},
	AbstractUI<QtBridgeUiRequest>("QtBridgeUi")

{

}

QtBridgeUi::~QtBridgeUi()
{

}

static QtBridgeUi* inst = new QtBridgeUi;

QtBridgeUi& QtBridgeUi::instance()
{
	return *inst;
}

void QtBridgeUi::destroy()
{
	delete inst;
	inst = nullptr;
}

void QtBridgeUi::thread_init()
{

}

void QtBridgeUi::do_request(QtBridgeUiRequest* req)
{
	if(req->type == BaseUI::CallSlot)
		req->the_slot();
}
