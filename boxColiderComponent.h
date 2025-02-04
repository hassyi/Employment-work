#pragma once
#include "colider.h"

class GameObject;

class BoxColiderComponent : public Colider
{
private:
	Box m_Box{ XMFLOAT3(0.0f,0.0f,0.0f),
			   XMFLOAT3(0.0f,0.0f,0.0f),
			  {XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3(0.0f,0.0f,0.0f) } };


public:
	BoxColiderComponent() = default;

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	std::tuple<bool, GameObject*, std::list<GameObject*>> GetCollision()override;
	std::tuple<bool, GameObject*, std::list<GameObject*>> GetCollisionOBB();

	void MoveCollision()override;

	Box GetOBB() { return m_Box; }

	void DrawImGui();

};