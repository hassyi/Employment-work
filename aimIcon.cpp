#include "aimIcon.h"
#include "textureComponent.h"
#include "texture2D.h"
#include "scene.h"
#include "game.h"
#include "player.h"

void AimIcon::Init()
{
	AddComponent<TextureComponent>()->SetTransTexNum(10.0f, 600.0f, 100.0f, 100.0f, L"asset\\texture\\aim.png", 1);
	GetComponent<TextureComponent>()->SetDisitDisplay(false);
	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void AimIcon::Uninit()
{
	for (auto component : m_ComponentList)
	{
		GetComponent<TextureComponent>()->Uninit();
		delete component;
	}
}

void AimIcon::Update()
{

}

void AimIcon::Draw()
{
	if (m_IsDraw)
	{
		for (auto component : m_ComponentList)
		{
			GetComponent<TextureComponent>()->SetPosSize(620.0f, 210.0f, 100.0f, 100.0f);
			GetComponent<TextureComponent>()->SetAnimation(0.0f, 0.0f, 1.0f, 1.0f);
			component->Draw();
		}
	}
}