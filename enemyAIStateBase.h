#pragma once
#include "enemyAIState.h"
#include "enemy.h"

class EnemyAIStateBase
{
protected:

	Enemy* m_Enemy;
	ENEMY_STATE m_State = STATE_NONE;

public:
	EnemyAIStateBase() = delete;
	EnemyAIStateBase(Enemy* enemy) { m_Enemy = enemy; }
	~EnemyAIStateBase(){}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;

	

};
