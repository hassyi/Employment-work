#pragma once
#include "texture.h"

class TextureComponent : public Texture
{
private:
	float m_Alpha = 1.0f;

public:
	TextureComponent() = default;

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetaAlpha(float alpha) { m_Alpha = alpha; }
	float GetAlpha() { return m_Alpha; }
};