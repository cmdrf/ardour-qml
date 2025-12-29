#ifndef TRIGGER_H
#define TRIGGER_H

#include "Stateful.h"
#include "FollowAction.h"

#include <ardour/triggerbox.h>

#include <QColor>

class Trigger : public Stateful
{
	Q_OBJECT

	Q_PROPERTY(bool armed READ armed WRITE setArmed NOTIFY armedChanged FINAL)
	Q_PROPERTY(LaunchStyle launchStyle READ launchStyle WRITE setLaunchStyle NOTIFY launchStyleChanged FINAL)
	Q_PROPERTY(FollowAction followAction0 READ followAction0 WRITE setFollowAction0 NOTIFY followAction0Changed FINAL)
	Q_PROPERTY(FollowAction followAction1 READ followAction1 WRITE setFollowAction1 NOTIFY followAction1Changed FINAL)
	Q_PROPERTY(int followActionProbability READ followActionProbability WRITE setFollowActionProbability NOTIFY followActionProbabilityChanged FINAL)
	Q_PROPERTY(int followCount READ followCount WRITE setFollowCount NOTIFY followCountChanged FINAL)
//	Q_PROPERTY(Temporal::BBT_Offset quantization READ quantization WRITE setQuantization NOTIFY quantizationChanged FINAL)
//	Q_PROPERTY(Temporal::BBT_Offset followLength READ followLength WRITE setFollowLength NOTIFY followLengthChanged FINAL)
	Q_PROPERTY(bool useFollowLength READ useFollowLength WRITE setUseFollowLength NOTIFY useFollowLengthChanged FINAL)
	Q_PROPERTY(bool legato READ legato WRITE setLegato NOTIFY legatoChanged FINAL)
	Q_PROPERTY(float velocityEffect READ velocityEffect WRITE setVelocityEffect NOTIFY velocityEffectChanged FINAL)
	Q_PROPERTY(bool stretchable READ stretchable WRITE setStretchable NOTIFY stretchableChanged FINAL)
	Q_PROPERTY(bool cueIsolated READ cueIsolated WRITE setCueIsolated NOTIFY cueIsolatedChanged FINAL)
	Q_PROPERTY(bool allowPatchChanges READ allowPatchChanges WRITE setAllowPatchChanges NOTIFY allowPatchChangesChanged FINAL)
	Q_PROPERTY(float gain READ gain WRITE setGain NOTIFY gainChanged FINAL)
	Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)

public:
	enum LaunchStyle
	{
		OneShotLaunchStyle = ARDOUR::Trigger::OneShot,
		ReTriggerLaunchStyle = ARDOUR::Trigger::ReTrigger,
		GateLaunchStyle = ARDOUR::Trigger::Gate,
		ToggleLaunchStyle = ARDOUR::Trigger::Toggle,
		RepeatLaunchStyle = ARDOUR::Trigger::Repeat
	};
	Q_ENUM(LaunchStyle);

	enum StretchMode
	{
		CrispStretchMode = ARDOUR::Trigger::Crisp,
		MixedStretchMode = ARDOUR::Trigger::Mixed,
		SmoothStretchMode = ARDOUR::Trigger::Smooth
	};
	Q_ENUM(StretchMode)

	Trigger(QObject* parent, std::shared_ptr<ARDOUR::Trigger> trigger);

	/// Create Trigger or a subclass from ARDOUR::Trigger or a subclass
	static Trigger* create(QObject* parent, std::shared_ptr<ARDOUR::Trigger> trigger);

	std::shared_ptr<ARDOUR::Trigger> trigger() {return std::dynamic_pointer_cast<ARDOUR::Trigger>(m_stateful);}
	const std::shared_ptr<ARDOUR::Trigger> trigger() const {return std::dynamic_pointer_cast<ARDOUR::Trigger>(m_stateful);}

	bool armed() const {return trigger()->armed();}
	void setArmed(bool newArmed);

	LaunchStyle launchStyle() const;
	void setLaunchStyle(const LaunchStyle& newLaunchStyle);
	FollowAction followAction0() const;
	void setFollowAction0(const FollowAction& newFollowAction0);
	FollowAction followAction1() const;
	void setFollowAction1(const FollowAction& newFollowAction1);
	int followActionProbability() const;
	void setFollowActionProbability(int newFollowActionProbability);
	int followCount() const;
	void setFollowCount(int newFollowCount);
	bool useFollowLength() const;
	void setUseFollowLength(bool newUseFollowLength);
	bool legato() const;
	void setLegato(bool newLegato);
	float velocityEffect() const;
	void setVelocityEffect(float newVelocityEffect);
	bool stretchable() const;
	void setStretchable(bool newStretchable);
	bool cueIsolated() const;
	void setCueIsolated(bool newCueIsolated);
	bool allowPatchChanges() const;
	void setAllowPatchChanges(bool newAllowPatchChanges);
	float gain() const;
	void setGain(float newGain);
	QColor color() const;
	void setColor(const QColor& newColor);

public Q_SLOTS:
	void bang(float velocity = 1.0f) {trigger()->bang(velocity);}
	void unbang() {trigger()->unbang();}
	void requestStop() {trigger()->request_stop();}
	void stopQuantized() {trigger()->stop_quantized();}

Q_SIGNALS:
	void armedChanged();
	void launchStyleChanged();
	void followAction0Changed();
	void followAction1Changed();
	void followActionProbabilityChanged();
	void followCountChanged();
	void useFollowLengthChanged();
	void legatoChanged();
	void velocityEffectChanged();
	void stretchableChanged();
	void cueIsolatedChanged();
	void allowPatchChangesChanged();
	void gainChanged();
	void colorChanged();

private Q_SLOTS:
	void updateProperties(const PBD::PropertyChange& change);

private:
};

#endif // TRIGGER_H
