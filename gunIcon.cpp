#include "gunIcon.h"
#include "textureComponent.h"
#include "texture2D.h"
#include "scene.h"
#include "game.h"
#include "player.h"

void GunIcon::Init()
{
	AddComponent<TextureComponent>()->SetTransTexNum(10.0f, 550.0f, 150.0f, 150.0f, L"asset\\texture\\gun.png", 1);
	GetComponent<TextureComponent>()->SetDisitDisplay(false);
	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void GunIcon::Uninit()
{
	for (auto component : m_ComponentList)
	{
		GetComponent<TextureComponent>()->Uninit();
		delete component;
	}
}

void GunIcon::Update()
{

}

void GunIcon::Draw()
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();

	if (player->GetWeaponType() == WEAPON_TYPE::GUN)
	{
		for (auto component : m_ComponentList)
		{
			GetComponent<TextureComponent>()->SetPosSize(10.0f, 550.0f, 150.0f, 150.0f);
			GetComponent<TextureComponent>()->SetAnimation(0.0f, 0.0f, 1.0f, 1.0f);
			component->Draw();

		}
	}
}
