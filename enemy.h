#pragma once
#include "gameObject.h"
#include "component.h"

typedef enum
{
	None = 0,
	Idle,
	Walk,
	FindPos,

	Max
}SearchState;

class Enemy : public GameObject 
{
private:
	Component* m_Component{};

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	int m_Life = 0;
	int m_CreateBulletFrame = 0;
	float m_GroundHeight = 0.0f;
	float m_Speed = 0.0f;

	int m_AttackFrame = 0;
	bool m_isAttack = false;

	SearchState m_State = SearchState::None;

public:

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void EnemyCollision();

	void SetSpeed(float speed) { m_Speed = speed; }
	float GetSpeed()const { return m_Speed; }


	void DrawImGui()override;

};