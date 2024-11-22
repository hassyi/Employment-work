#pragma once
#include "gameObject.h"
#include "UI.h"

class Time : public UI
{
private:
	int m_timeCount = 0;
	int m_Count = 0;
	const int SCORE_DISIT = 3;

public:
	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void AddScore(int score) {	m_Count += score;	}
};