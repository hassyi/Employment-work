#pragma once

#include "enemyAIStateBase.h"

class ChasePlayerState : public EnemyAIStateBase
{
private:
	typedef enum
	{
		NONE, 
		IDLE,
		CHASE,

		MAX
	}CHASE_STATE;

	CHASE_STATE m_State = CHASE_STATE::IDLE;

	float m_AngleDegrees = 0.0f;

public:
	ChasePlayerState() = default;
	~ChasePlayerState(){}

	void Init()override;
	void Uninit()override;
	void Update()override;

};