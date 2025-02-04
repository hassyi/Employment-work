#pragma once
#include "enemyAIStateBase.h"

class EnemyAttackState : public EnemyAIStateBase
{
private:
	int m_AttackFrame = 0;
	bool m_isAttack = false;

public:
	EnemyAttackState() = default;
	~EnemyAttackState(){}

	void Init()override;
	void Uninit()override;
	void Update(Enemy* enemy)override;


};
