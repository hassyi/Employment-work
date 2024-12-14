#include "TitleEffect.h"
#include "textureComponent.h"

void TitleEffect::Init()
{
	AddComponent<TextureComponent>()->SetTransTexNum(0.0f, 0.0f, 100.0f, 100.0f, L"asset\\texture\\wave.png", 1);
	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void TitleEffect::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
	}
}

void TitleEffect::Update()
{

}

void TitleEffect::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}
