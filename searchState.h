#pragma once
#include "enemyAIStateBase.h"

class SearchState : public EnemyAIStateBase
{
private:

	float m_AngleDegrees = 0.0f;

public:
	using EnemyAIStateBase::EnemyAIStateBase;
	void Init()override;
	void Uninit()override;
	void Update()override;


};