#pragma once
#include "gameObject.h"
#include "component.h"

class EnemySlash : public GameObject
{
private:
	XMFLOAT3 m_Rot = {};

	int m_frame = 0;
	bool m_isHit = false;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SlashCollision();

	void SetRot(XMFLOAT3 rot) { m_Rot = rot; }
};