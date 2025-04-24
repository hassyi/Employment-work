#pragma once
#include "enemyAIStateBase.h"

class AttackState : public EnemyAIStateBase
{
private:
	class BehaviorNode* m_RootNode;

public:
	using EnemyAIStateBase::EnemyAIStateBase;
	void Init()override;
	void Uninit()override;
	void Update()override;


};