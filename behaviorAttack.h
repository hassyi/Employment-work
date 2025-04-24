#pragma once
#include "behaviorNode.h"

enum ATTACK_STATE
{
	ATTACK_NONE,
	PUNCH,
	GUN,
};


class BehaviorAttack : public BehaviorNode
{
private:
	ATTACK_STATE m_Attack = ATTACK_NONE;


public:
	using BehaviorNode::BehaviorNode;
	//BEHAVIOR_RESULT Update()override;

};
