#pragma once
#include "UI.h"


class Texture2D : public UI
{
private:

	XMFLOAT2 m_Pos = {};
	XMFLOAT2 m_Scale = {};

	const wchar_t* m_TextureData = L"none";

public:

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;



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
