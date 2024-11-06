#pragma once
#include "gameObject.h"

class Time : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	int m_timeCount = 0;
	int m_Count = 0;
	const int SCORE_DISIT = 3;

public:
	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void AddScore(int score) {	m_Count += score;	}
};