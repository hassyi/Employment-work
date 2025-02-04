#pragma once
#include "behaviorNode.h"

class BehaviorSearch : public BehaviorNode
{
private:
	float m_AngleDegrees = 0.0f;

public:
	using BehaviorNode::BehaviorNode;
	BEHAVIOR_RESULT Update()override;
	
};