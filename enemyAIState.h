#pragma once
#include "gameObjectComponent.h"


enum ENEMY_STATE
{
	STATE_NONE = 0,
	STATE_IDLE,
	STATE_SEARCH,
	STATE_BATTLE,
	STATE_MAX
};


class EnemyAIState : public GameObjectComponet
{
protected:
	class Enemy* m_Enemy = nullptr;
	ENEMY_STATE m_State = STATE_NONE;
	class EnemyAIStateBase* m_EnemyAI = nullptr;

public:
	EnemyAIState() = default;
	EnemyAIState(Enemy* enemy) { m_Enemy = enemy; }
	~EnemyAIState() {}

	void SetEnemy(Enemy* enemy);
	void Init()override;
	void Uninit()override;
	void Update()override;

	void ChangeState(ENEMY_STATE state);

};