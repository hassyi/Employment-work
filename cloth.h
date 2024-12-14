#pragma once
#include "gameobject.h"

#define VERTEX_X (10)
#define VERTEX_Y (10)

#define SPRING_NUM (VERTEX_X*(VERTEX_Y + 1) + \
					VERTEX_Y*(VERTEX_X + 1) + \
					VERTEX_Y*VERTEX_X*2)
#define DELTA_TIME 0.1f

class Cloth : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	const float ATT_COEFFICIENT = 0.5f;		//減衰係数
	const float SPRING_MASS = 2.0f;
	float m_Time = 0.0f;
	int m_Status = 0;

	struct VERTEX
	{
		VERTEX_3D vertex;
		XMFLOAT3 vel;					//速度
		XMFLOAT3 asccel;					//加速度
		XMFLOAT3 resultant;				//合力
		XMFLOAT3 gravity;				//重力
		bool OnLook;
	};

	struct SPRING
	{
		XMFLOAT2 p1;
		XMFLOAT2 p2;
		float length;
	};

	VERTEX m_Vertex[VERTEX_Y + 1][VERTEX_X + 1];
	SPRING m_Spring[SPRING_NUM];
	XMFLOAT3 m_WindForce;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;


};
