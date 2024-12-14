#include "texture2D.h"
#include "textureComponent.h"

void Texture2D::Init()
{
	AddComponent<TextureComponent>()->SetTransTexNum(m_Pos.x, m_Pos.y, m_Scale.x, m_Scale.y, m_TextureData, m_TextureNum);

	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void Texture2D::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Texture2D::Update()
{
	for (auto component : m_ComponentList)
	{
		component->Update();
	}
}

void Texture2D::Draw()
{
	if (m_isDraw)
	{
		for (auto component : m_ComponentList)
		{
			component->Draw();
		}
	}
}
