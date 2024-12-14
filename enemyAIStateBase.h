#pragma once
#include "main.h"

class EnemyAIStateBase
{
protected:


public:
	EnemyAIStateBase() = default;
	~EnemyAIStateBase(){}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;

	

};
