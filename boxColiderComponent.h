#pragma once
#include "colider.h"
#include "gameObject.h"


class BoxColiderComponent : public Colider
{
private:



public:
	BoxColiderComponent() = default;

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	std::tuple<bool, GameObject*, std::list<GameObject*>> GetCollision()override;
	std::tuple<bool, GameObject*, std::list<GameObject*>> GetCollisionOBB();

};