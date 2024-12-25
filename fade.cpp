#include "main.h"
#include "renderer.h"
#include "fade.h"
#include "textureComponent.h"


void Fade::Init()
{
	AddComponent<TextureComponent>()->SetTransTexNum(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, L"asset\\texture\\fade.png", 1);
	GetComponent<TextureComponent>()->SetaAlpha(0.0f);

	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void Fade::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
	}

}

void Fade::Update()
{
	if (m_FadeState == FADE_STATE::FADE_OUT)
	{
		m_Alpha += 0.01f;
		GetComponent<TextureComponent>()->SetaAlpha(m_Alpha);
		if (m_Alpha >= 1.0f) {
			m_FadeState = FADE_STATE::FADE_NONE;
		}
	}
	else if (m_FadeState == FADE_STATE::FADE_IN)
	{
		m_Alpha -= 0.01f;
		GetComponent<TextureComponent>()->SetaAlpha(m_Alpha);
		if (m_Alpha <= 0.0f) {
			m_FadeState = FADE_STATE::FADE_NONE;
		}
	}
}

void Fade::Draw()
{
	if (m_OnDraw)
	{
		for (auto component : m_ComponentList)
		{
			component->Draw();
		}
	}
}
