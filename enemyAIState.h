#pragma once
#include "gameObjectComponent.h"
#include "enemy.h"


typedef enum
{
	STATE_NONE = 0,
	STATE_IDLE,
	STATE_CHASE,
	STATE_ATTACK,
	STATE_DODGE,

	STATE_MAX
}ENEMY_STATE;

class EnemyAIStateBase;
class EnemyAttackState;
class ChasePlayerState;
class EnemyDodgeState;
//class EnemyIdleState;

class EnemyAIState : public GameObjectComponet
{
private:
	EnemyAIStateBase* m_State = nullptr;
	EnemyAttackState* m_AttackState = nullptr;
	ChasePlayerState* m_ChaseState = nullptr;
	EnemyDodgeState* m_DodgeState = nullptr;
	//EnemyIdleState* m_IdleState = nullptr;

public:
	EnemyAIState() = default;
	~EnemyAIState() {}

	void Init();
	void Uninit();
	void Update(Enemy* enemy);

	void ChangeState(ENEMY_STATE state);

};