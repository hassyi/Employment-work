#pragma once
#include "gameObject.h"
#include "UI.h"

class TitleName : public UI
{
private:
	int m_timeCount = 0;
	int m_Count = 0;
	const int SCORE_DISIT = 3;

	float m_Alpha = 0.0f;

public:
	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void AddScore(int score) {	m_Count += score;	}
};