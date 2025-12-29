#include "FollowAction.h"


FollowAction::FollowAction(const ARDOUR::FollowAction& followAction) :
	m_followAction(followAction)
{

}

FollowAction::Type FollowAction::type() const
{
	return static_cast<Type>(m_followAction.type);
}

void FollowAction::setType(const Type& newType)
{
	m_followAction.type = static_cast<ARDOUR::FollowAction::Type>(newType);
}
