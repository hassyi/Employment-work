#pragma once
#include "behaviorNode.h"

class BehaviorSequence : public BehaviorNode
{
private:
	int m_Index = 0;

public:
	using BehaviorNode::BehaviorNode;
	BEHAVIOR_RESULT Update()override;
	int GetIndex() { return m_Index; }
};