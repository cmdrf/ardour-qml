#ifndef FOLLOWACTION_H
#define FOLLOWACTION_H

#include <ardour/types.h>
#include <QObject>

class FollowAction
{
	Q_GADGET

	Q_PROPERTY(Type type READ type WRITE setType FINAL)
	// TODO: targets

public:
	enum Type
	{
		NoneType = ARDOUR::FollowAction::None,
		StopType = ARDOUR::FollowAction::Stop,
		AgainType = ARDOUR::FollowAction::Again,
		ForwardTriggerType = ARDOUR::FollowAction::ForwardTrigger,
		ReverseTriggerType = ARDOUR::FollowAction::ReverseTrigger,
		FirstTriggerType = ARDOUR::FollowAction::FirstTrigger,
		LastTriggerType = ARDOUR::FollowAction::LastTrigger,
		JumpTriggerType = ARDOUR::FollowAction::JumpTrigger
	};
	Q_ENUM(Type);

	FollowAction(const ARDOUR::FollowAction& followAction);

	operator ARDOUR::FollowAction() const {return m_followAction;}

	Type type() const;
	void setType(const Type& newType);

private:
	ARDOUR::FollowAction m_followAction;
};

#endif // FOLLOWACTION_H
