#pragma once
#include "behaviorNode.h"

class BehaviorIdle : public BehaviorNode
{
private:

public:
	using BehaviorNode::BehaviorNode;
	BEHAVIOR_RESULT Update()override;

};