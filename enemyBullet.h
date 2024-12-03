#pragma once
#include "gameObject.h"
#include "component.h"

class EnemyBullet : public GameObject
{
private:

	int m_Frame = 0;
	XMFLOAT3 m_PlayerPos{};
	XMFLOAT3 m_Launchpoint{};

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void EnemyBulletCollision();
};