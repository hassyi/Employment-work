#pragma once
#include "texture.h"

class TextureComponent : public Texture
{
private:
	XMFLOAT4 m_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	float m_Alpha = 1.0f;

public:
	TextureComponent() = default;

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetaAlpha(float alpha) { m_Diffuse.w = alpha; }
	float GetAlpha() { return m_Diffuse.w; }
	void SetDiffuse(XMFLOAT4 diffuse) { m_Diffuse = diffuse; }
};