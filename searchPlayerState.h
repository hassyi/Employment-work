#pragma once
#include "enemyAIStateBase.h"


typedef enum
{
	None = 0, 
	Idle,
	Walk,
	FindPos,

	Max
}SearchState;

class SearchPlayerState : public EnemyAIStateBase
{
private:
	SearchState m_State = SearchState::None;

public:
	SearchPlayerState() = default;
	~SearchPlayerState(){}

	void Init()override;
	void Uninit()override;
	void Update()override;


};