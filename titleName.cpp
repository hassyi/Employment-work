#include "titleName.h"
#include "textureComponent.h"
#include "texture2D.h"

void TitleName::Init()
{
	AddComponent<TextureComponent>()->SetTransTexNum(150.0f, 80.0f, 350.0f, 150.0f, L"asset\\texture\\titlename.png", 1);
	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void TitleName::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();

		delete component;
	}
}

void TitleName::Update()
{
	m_Alpha+=0.01f;

	if (m_Alpha >= 1.0f)
	{
		m_Alpha = 1.0f;
	}
	GetComponent<TextureComponent>()->SetaAlpha(m_Alpha);
}

void TitleName::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
		
	}
}

