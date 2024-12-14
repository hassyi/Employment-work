#pragma once
#include "enemyAIStateBase.h"

class EnemyAIState
{
private:
	EnemyAIStateBase* m_State = nullptr;


public:
	EnemyAIState() = default;
	~EnemyAIState() {}

	void Init() { m_State->Init(); }
	void Uninit() { m_State->Uninit(); }
	void Update() { m_State->Update(); }
	void Draw() {}

	void ChangeState(EnemyAIStateBase* state) 
	{
		delete m_State; 
		m_State = state;
		Update();
	}

};