#pragma once
#include "colider.h"
#include "gameObject.h"

class SphereColiderComponent : public Colider
{
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	std::tuple<bool, GameObject*, std::list<GameObject*>> GetCollision()override;

};
