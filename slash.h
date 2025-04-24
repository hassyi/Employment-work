#pragma once
#include "gameObject.h"
#include "component.h"

class Slash : public GameObject
{
private:
	XMFLOAT3 m_Rot = {};

	class SlashEffect* m_Effect = nullptr;

	int m_EffectNum = 0;
	int m_frame = 0;
	bool m_isHit = false;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SlashCollision();

	void SetRot(XMFLOAT3 rot) { m_Rot = rot; }
	void SetEffectNum(int num) { m_EffectNum = num; }
	void SetEffect();
};