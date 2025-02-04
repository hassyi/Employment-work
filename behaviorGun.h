#pragma once
#include "behaviorNode.h"

class BehaviorGun : public BehaviorNode
{
private:
	int m_ShotFrame = 0;

public:
	using BehaviorNode::BehaviorNode;
	BEHAVIOR_RESULT Update()override;
};