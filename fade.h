#pragma once
#include "UI.h"

typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT

}FADE_STATE;

class Fade : public UI
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	XMFLOAT2 m_Pos{};
	XMFLOAT2 m_Size{};

	float m_Alpha = 0.0f;
	bool m_OnDraw = false;

	const wchar_t* m_TextureData = L"none";

	FADE_STATE m_FadeState = FADE_STATE::FADE_NONE;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetFade(bool onDraw) { m_OnDraw = onDraw; }
	void SetFadeState(FADE_STATE state) { m_FadeState = state; }
	FADE_STATE GetFadeState() { return m_FadeState; }

	void SetTransTexNum(float posx, float posy, float sizex, float sizey, const wchar_t* data, int num)
	{
		m_Pos.x = posx;
		m_Pos.y = posy;
		m_Size.x = sizex;
		m_Size.y = sizey;
		m_TextureData = data;
		m_TextureNum = num;
	}
};