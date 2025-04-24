#pragma once
#include "gameObject.h"

class SlashEffect : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_Count{};
	bool m_IsDraw = false;

	int m_TexNum = 0;
	const wchar_t* m_TextureData = L"none";


public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetTexNum(int num) { m_TexNum = num; }
};