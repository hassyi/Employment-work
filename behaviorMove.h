#pragma once
#include "behaviorNode.h"

class BehaviorMove : public BehaviorNode
{
private:


public:
	using BehaviorNode::BehaviorNode;
	BEHAVIOR_RESULT Update()override;
};
