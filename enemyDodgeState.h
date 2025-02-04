#pragma once

#include "enemyAIStateBase.h"

class EnemyDodgeState : public EnemyAIStateBase
{
private:
	bool m_isDodge = false;
	int m_DodgeFrame = 0.0f;

public:
	EnemyDodgeState() = default;
	~EnemyDodgeState() {}

	void Init()override;
	void Uninit()override;
	void Update(Enemy* enemy)override;


};