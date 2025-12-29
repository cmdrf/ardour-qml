#include "Trigger.h"
#include "QtBridgeUi.h"

#include <ardour/presentation_info.h>


Trigger::Trigger(QObject* parent, std::shared_ptr<ARDOUR::Trigger> trigger)
	: Stateful{parent, trigger}
{
	QtBridgeUi& b = QtBridgeUi::instance();

	b.connect(trigger->PropertyChanged, this, &Trigger::updateProperties);
}

void Trigger::setArmed(bool newArmed)
{
	if(newArmed)
		trigger()->arm();
	else
		trigger()->disarm();
}

void Trigger::updateProperties(const PBD::PropertyChange& change)
{
	if(change.contains(ARDOUR::Properties::launch_style))
		Q_EMIT launchStyleChanged();
	if(change.contains(ARDOUR::Properties::follow_action0))
		Q_EMIT followAction0Changed();
	if(change.contains(ARDOUR::Properties::follow_action1))
		Q_EMIT followAction1Changed();
	if(change.contains(ARDOUR::Properties::follow_action_probability))
		Q_EMIT followActionProbabilityChanged();
	if(change.contains(ARDOUR::Properties::follow_count))
		Q_EMIT followCountChanged();
	if(change.contains(ARDOUR::Properties::use_follow_length))
		Q_EMIT useFollowLengthChanged();
	if(change.contains(ARDOUR::Properties::legato))
		Q_EMIT legatoChanged();
	if(change.contains(ARDOUR::Properties::velocity_effect))
		Q_EMIT velocityEffectChanged();
	if(change.contains(ARDOUR::Properties::stretchable))
		Q_EMIT stretchableChanged();
	if(change.contains(ARDOUR::Properties::cue_isolated))
		Q_EMIT cueIsolatedChanged();
	if(change.contains(ARDOUR::Properties::allow_patch_changes))
		Q_EMIT allowPatchChangesChanged();
	if(change.contains(ARDOUR::Properties::gain))
		Q_EMIT gainChanged();
//	if(change.contains(ARDOUR::Properties::stretch_mode))
//		Q_EMIT stretchModeChanged();
	if(change.contains(ARDOUR::Properties::color))
		Q_EMIT colorChanged();
}

Trigger::LaunchStyle Trigger::launchStyle() const
{
	return static_cast<LaunchStyle>(trigger()->launch_style());
}

void Trigger::setLaunchStyle(const LaunchStyle& newLaunchStyle)
{
	trigger()->set_launch_style(static_cast<ARDOUR::Trigger::LaunchStyle>(newLaunchStyle));
}

FollowAction Trigger::followAction0() const
{
	return trigger()->follow_action0();
}

void Trigger::setFollowAction0(const FollowAction& newFollowAction0)
{
	trigger()->set_follow_action0(newFollowAction0);
}

FollowAction Trigger::followAction1() const
{
	return trigger()->follow_action1();
}

void Trigger::setFollowAction1(const FollowAction& newFollowAction1)
{
	trigger()->set_follow_action1(newFollowAction1);
}

int Trigger::followActionProbability() const
{
	return trigger()->follow_action_probability();
}

void Trigger::setFollowActionProbability(int newFollowActionProbability)
{
	trigger()->set_follow_action_probability(newFollowActionProbability);
}

int Trigger::followCount() const
{
	return trigger()->follow_count();
}

void Trigger::setFollowCount(int newFollowCount)
{
	trigger()->set_follow_count(newFollowCount);
}

bool Trigger::useFollowLength() const
{
	return trigger()->use_follow_length();
}

void Trigger::setUseFollowLength(bool newUseFollowLength)
{
	trigger()->set_use_follow_length(newUseFollowLength);
}

bool Trigger::legato() const
{
	return trigger()->legato();
}

void Trigger::setLegato(bool newLegato)
{
	trigger()->set_legato(newLegato);
}

float Trigger::velocityEffect() const
{
	return trigger()->velocity_effect();
}

void Trigger::setVelocityEffect(float newVelocityEffect)
{
	trigger()->set_velocity_effect(newVelocityEffect);
}

bool Trigger::stretchable() const
{
	return trigger()->stretchable();
}

void Trigger::setStretchable(bool newStretchable)
{
	trigger()->set_stretchable(newStretchable);
}

bool Trigger::cueIsolated() const
{
	return trigger()->cue_isolated();
}

void Trigger::setCueIsolated(bool newCueIsolated)
{
	trigger()->set_cue_isolated(newCueIsolated);
}

bool Trigger::allowPatchChanges() const
{
	return trigger()->allow_patch_changes();
}

void Trigger::setAllowPatchChanges(bool newAllowPatchChanges)
{
	trigger()->set_allow_patch_changes(newAllowPatchChanges);
}

float Trigger::gain() const
{
	return trigger()->gain();
}

void Trigger::setGain(float newGain)
{
	trigger()->set_gain(newGain);
}

/*
Trigger::StretchMode Trigger::stretchMode() const
{
	return static_cast<StretchMode>(trigger()->stretch_mode());
}

void Trigger::setStretchMode(const StretchMode& newStretchMode)
{
	trigger()->set_stretch_mode(static_cast<ARDOUR::Trigger::StretchMode>(newStretchMode));
}
*/

QColor Trigger::color() const
{
	return QColor::fromRgba(trigger()->color());
}

void Trigger::setColor(const QColor& newColor)
{
	trigger()->set_color(newColor.rgb());
}
