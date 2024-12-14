#pragma once
#include "gameObject.h"

#define SIZE_NUM (53)

class Wave : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	ID3D11ShaderResourceView* m_EnvTexture{};			//環境マッピング

	VERTEX_3D m_Vertex[SIZE_NUM][SIZE_NUM];

	float m_Time = 0.0f;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};