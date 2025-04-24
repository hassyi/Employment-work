#pragma once

#include "enemyAIStateBase.h"

class IdleState : public EnemyAIStateBase
{
private:


public:
	using EnemyAIStateBase::EnemyAIStateBase;
	void Init()override;
	void Uninit()override;
	void Update()override;


};