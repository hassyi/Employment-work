#pragma once
#include "behaviorNode.h"

class BehaviorPunch : public BehaviorNode
{
private:
	float m_AttackFrame = 0.0f;
	bool m_isAttack = false;
	int m_AttackCount = 0;

public:
	using BehaviorNode::BehaviorNode;
	BEHAVIOR_RESULT Update()override;
};