#include "swordIcon.h"
#include "textureComponent.h"
#include "texture2D.h"
#include "scene.h"
#include "game.h"
#include "player.h"

void SwordIcon::Init()
{
	AddComponent<TextureComponent>()->SetTransTexNum(10.0f, 600.0f, 100.0f, 100.0f, L"asset\\texture\\sword.png", 1);
	GetComponent<TextureComponent>()->SetDisitDisplay(false);
	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void SwordIcon::Uninit()
{
	for (auto component : m_ComponentList)
	{
		GetComponent<TextureComponent>()->Uninit();
		delete component;
	}
}

void SwordIcon::Update()
{

}

void SwordIcon::Draw()
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();

	if (player->GetWeaponType() == WEAPON_TYPE::SWORD)
	{
		for (auto component : m_ComponentList)
		{
			GetComponent<TextureComponent>()->SetPosSize(10.0f, 600.0f, 100.0f, 100.0f);
			GetComponent<TextureComponent>()->SetAnimation(0.0f, 0.0f, 1.0f, 1.0f);
			component->Draw();

		}
	}
}
