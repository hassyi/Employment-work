#pragma once
#include "gameObject.h"
#include "component.h"

class Sword : public GameObject
{
private:

	int m_frame = 0;
	bool m_isHit = false;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SwordCollision();
};