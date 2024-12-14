#pragma once
#include "UI.h"

class TitleEffect : public UI
{
private:
	XMFLOAT2 m_Pos = {};
	XMFLOAT2 m_Size = {};

public:
	TitleEffect() {}
	~TitleEffect(){}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

};