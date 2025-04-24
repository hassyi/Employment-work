#include "enemyAIState.h"
#include "idleState.h"
#include "searchState.h"
#include "battleState.h"

void EnemyAIState::SetEnemy(Enemy* enemy)
{
	m_Enemy = enemy;
}

void EnemyAIState::Init()
{
	m_EnemyAI = new IdleState(m_Enemy);
}

void EnemyAIState::Uninit()
{
	m_EnemyAI->Uninit();
	delete m_EnemyAI;
}

void EnemyAIState::Update()
{
	m_EnemyAI->Update();
}

void EnemyAIState::ChangeState(ENEMY_STATE state)
{	
	m_EnemyAI->Uninit();
	delete m_EnemyAI;
	m_State = state;
	switch (m_State)
	{
	case STATE_IDLE:
		m_EnemyAI = new IdleState(m_Enemy);
		break;
	case STATE_SEARCH:
		m_EnemyAI = new SearchState(m_Enemy);
		break;
	case STATE_BATTLE:
		m_EnemyAI = new BattleState(m_Enemy);
		m_EnemyAI->Init();
		break;
	default:
		break;
	}
}
