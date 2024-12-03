#pragma once
#include "gameObject.h"
#include "component.h"

class Enemy : public GameObject {
private:
	Component* m_Component{};

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	int m_life = 0;
	int m_CreateBulletFrame = 0;
	float m_GroundHeight = 0.0f;

public:

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void EnemyCollision();

};