#pragma once
#include "UI.h"
#include "uiComponent.h"


class Texture2D : public UI
{
private:

	XMFLOAT2 m_Pos = {};
	XMFLOAT2 m_Scale = {};

	const wchar_t* m_TextureData = L"none";

	bool m_isDraw = true;

public:

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetIsDraw(bool isDraw) { m_isDraw = isDraw; }

	void SetTransTexNum(float posx, float posy, float sizex, float sizey, const wchar_t* data, int num)
	{
		m_Pos.x = posx;
		m_Pos.y = posy;
		m_Scale.x = sizex;
		m_Scale.y = sizey;
		m_TextureData = data;
		m_TextureNum = num;
	}
};
