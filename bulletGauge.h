#pragma once
#include "gameObject.h"
#include "UI.h"

class BulletGauge : public UI
{
private:
	float m_BP_Max = 0.0f;

public:
	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};