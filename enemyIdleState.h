#pragma once
#include "enemyAIStateBase.h"


class EnemyIdleState : public EnemyAIStateBase
{
private:

public:
	EnemyIdleState() = default;
	~EnemyIdleState(){}

	void Init()override;
	void Uninit()override;
	void Update(Enemy* enemy)override;


};