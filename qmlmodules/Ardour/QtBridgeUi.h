#ifndef QTBRIDGEUI_H
#define QTBRIDGEUI_H

#include <QObject>

#include <pbd/abstract_ui.h>

struct QtBridgeUiRequest : public BaseUI::BaseRequestObject {
  public:
	QtBridgeUiRequest () { }
	~QtBridgeUiRequest() { }
};

class QtBridgeUi : public QObject, public AbstractUI<QtBridgeUiRequest>
{
	Q_OBJECT
	Q_DISABLE_COPY(QtBridgeUi)

public:
	explicit QtBridgeUi(QObject *parent = nullptr);
	~QtBridgeUi() {}; // Required for some reason

	static QtBridgeUi& instance();

	void connect(PBD::Signal<void ()>& signal, QObject *receiver, const char *method);

protected:
	void thread_init();
	void do_request(QtBridgeUiRequest*);

private:
//	PBD::ScopedConnectionList& connectionList;
};

#endif // QTBRIDGEUI_H
