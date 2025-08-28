#pragma once
#include "gameObject.h"
#include "UI.h"

class StaminaGauge : public UI
{
private:
	float m_ST_Max = 0.0f;

public:
	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};