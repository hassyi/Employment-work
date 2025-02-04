#include "weaponIcon.h"
#include "textureComponent.h"
#include "texture2D.h"

void WeaponIcon::Init()
{
	AddComponent<TextureComponent>()->SetTransTexNum(10.0f, 600.0f, 100.0f, 100.0f, L"asset\\texture\\sword.png", 1);
	GetComponent<TextureComponent>()->SetDisitDisplay(false);

	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void WeaponIcon::Uninit()
{
	for (auto component : m_ComponentList)
	{
		GetComponent<TextureComponent>()->Uninit();
		delete component;
	}
}

void WeaponIcon::Update()
{

}

void WeaponIcon::Draw()
{
	for (auto component : m_ComponentList)
	{
		GetComponent<TextureComponent>()->SetPosSize(10.0f, 600.0f, 100.0f, 100.0f);
		GetComponent<TextureComponent>()->SetAnimation(0.0f, 0.0f, 1.0f, 1.0f);
		component->Draw();

	}
}
