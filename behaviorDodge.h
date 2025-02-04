#pragma once
#include "behaviorNode.h"

class BehaviorDodge : public BehaviorNode
{
private:
	bool m_isDodge = false;
	int m_DodgeFrame = 0.0f;

public:
	using BehaviorNode::BehaviorNode;
	BEHAVIOR_RESULT Update()override;
};