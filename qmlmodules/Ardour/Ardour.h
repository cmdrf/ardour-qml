#ifndef ARDOUR_H
#define ARDOUR_H

#include "Session.h"
#include "PluginManager.h"

#include <QObject>

class Ardour : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	Q_DISABLE_COPY(Ardour)

	Q_PROPERTY(Session* session READ session NOTIFY sessionChanged FINAL)
	Q_PROPERTY(PluginManager* pluginManager READ pluginManager CONSTANT FINAL)

public:
	// Enums from types.h go here:

	enum Placement
	{
		PreFaderPlacement = ARDOUR::PreFader,
		PostFaderPlacement = ARDOUR::PostFader
	};
	Q_ENUM(Placement);

	enum AlignStyle
	{
		CaptureTimeAlignStyle = ARDOUR::CaptureTime,
		ExistingMaterialAlignStyle = ARDOUR::ExistingMaterial
	};
	Q_ENUM(AlignStyle);

	explicit Ardour(QObject* parent = nullptr);
	~Ardour();

	Session* session() const;
	PluginManager* pluginManager() {return m_pluginManager;}

	Q_INVOKABLE bool createSession(const QString& dir, const QString& snapshotName, uint32_t sampleRate);
	Q_INVOKABLE bool loadSession(const QString& dir, const QString& snapshotName = QString());
	Q_INVOKABLE bool loadSession(const QUrl& ardourFile);
	Q_INVOKABLE void closeSession();

Q_SIGNALS:
	void sessionChanged();

private:
	Session* m_session = nullptr;
	PluginManager* m_pluginManager;
};

#endif // ARDOUR_H
