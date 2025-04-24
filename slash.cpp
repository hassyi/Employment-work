#include "slash.h"
#include "modelRenderer.h"
#include "scene.h"
#include "player.h"
#include "transform3DComponent.h"
#include "game.h"
#include "enemy.h"
#include "capsuleColiderComponent.h"
#include "slashEffect.h"
#include "transform2DComponent.h"

void Slash::Init()
{
	AddComponent<Transform3DComponent>();
	AddComponent<CapsuleColiderComponent>();

	m_ObjType = OBJ_TYPE::SLASH;

	for (auto component : m_ComponentList)
	{
		component->Init();
	}

	Scene::GetInstance()->GetScene<Game>()->AddGameObject<SlashEffect>(1);
	GetComponent<CapsuleColiderComponent>()->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	GetComponent<CapsuleColiderComponent>()->SetSegmentLength(1.0f);
}

void Slash::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Slash::Update()
{
	SlashEffect* effect = Scene::GetInstance()->GetScene<Game>()->GetGameObject<SlashEffect>();

	++m_frame;

	if (m_frame >= 20) {
		m_isHit = false;
		effect->SetDestroy();
		SetDestroy();
		return;
	}

	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	XMFLOAT3 playerpos = player->GetComponent<Transform>()->GetPos();
	XMFLOAT3 playerrot = player->GetComponent<Transform>()->GetRot();

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();

	pos.x = playerpos.x + 2.0f * cosf(playerrot.y - (XM_PI / 2));
	pos.z = playerpos.z + 2.0f * sinf(playerrot.y + (XM_PI / 2));
	pos.y = playerpos.y + 1.0f;

	GetComponent<Transform3DComponent>()->SetPos(pos);
	GetComponent<Transform3DComponent>()->SetRot(Add(m_Rot, playerrot));
	GetComponent<CapsuleColiderComponent>()->SetPos(GetComponent<Transform3DComponent>()->GetPos());
	GetComponent<CapsuleColiderComponent>()->SetRot(GetComponent<Transform3DComponent>()->GetRot());
	effect->GetComponent<Transform2DComponent>()->SetPos(pos);
	effect->GetComponent<Transform2DComponent>()->SetRot(GetComponent<Transform3DComponent>()->GetRot());


	SlashCollision();



	for (auto component : m_ComponentList)
	{
		component->Update();
	}

}

void Slash::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}

void Slash::SlashCollision()
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();

	if (std::get<0>(GetColider()->GetCollision()))
	{
		std::list<GameObject*> objectList = std::get<2>(GetComponent<Colider>()->GetCollision());
		for (auto onCollisionObject : objectList)
		{
			if (onCollisionObject->GetObjectType() == OBJ_TYPE::ENEMY)
			{
				if (!m_isHit)
				{
					player->SetBulletPoint(player->GetBulletPoint() + 10.0f);
					onCollisionObject->SetLife(onCollisionObject->GetLife() - 1);
					m_isHit = true;
				}
			}
		}
		GetComponent<Transform3DComponent>()->SetPos(pos);
	}
}