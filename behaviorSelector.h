#pragma once
#include "behaviorNode.h"

class BehaviorSelector : public BehaviorNode
{
private:
	int m_Index = 0;
		
public:
	using BehaviorNode::BehaviorNode;
	BEHAVIOR_RESULT Update()override;
};