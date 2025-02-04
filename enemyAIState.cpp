#include "enemyAIState.h"
#include "enemyAttackState.h"
#include "chasePlayerState.h"
#include "enemyIdleState.h"
#include "enemyDodgeState.h"

void EnemyAIState::Init()
{	
	m_AttackState = new EnemyAttackState;
	m_ChaseState = new ChasePlayerState;
	m_DodgeState = new EnemyDodgeState;
	//m_IdleState = new EnemyIdleState;
	m_State = m_ChaseState;
}

void EnemyAIState::Uninit()
{
	m_State->Uninit();
}

void EnemyAIState::Update(Enemy* enemy)
{
	m_State->Update(enemy);
}

void EnemyAIState::ChangeState(ENEMY_STATE state)
{
	m_State = nullptr;

	//if (state == ENEMY_STATE::STATE_IDLE)
	//{
	//	m_State = m_IdleState;
	//}
	if (state == ENEMY_STATE::STATE_CHASE)
	{
		m_State = m_ChaseState;
	}
	else if (state == ENEMY_STATE::STATE_ATTACK)
	{
		m_State = m_AttackState;
	}
	else if (state == ENEMY_STATE::STATE_DODGE)
	{
		m_State = m_DodgeState;
	}

}
