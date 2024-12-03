#pragma once
#include "gameObject.h"
#include "component.h"

class Bullet : public GameObject
{
private:

	int m_frame = 0;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void BulletCollision();
};