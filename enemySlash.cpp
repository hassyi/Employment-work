#include "enemySlash.h"
#include "modelRenderer.h"
#include "manager.h"
#include "player.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "game.h"
#include "enemy.h"
#include "capsuleColiderComponent.h"
#include "enemySlashEffect.h"

void EnemySlash::Init()
{
	AddComponent<Transform3DComponent>();
	AddComponent<CapsuleColiderComponent>();

	m_ObjType = OBJ_TYPE::SLASH;

	for (auto component : m_ComponentList)
	{
		component->Init();
	}

	Scene::GetInstance()->GetScene<Game>()->AddGameObject<EnemySlashEffect>(1);
	GetComponent<CapsuleColiderComponent>()->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	GetComponent<CapsuleColiderComponent>()->SetSegmentLength(1.0f);
}

void EnemySlash::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void EnemySlash::Update()
{
	EnemySlashEffect* effect = Scene::GetInstance()->GetScene<Game>()->GetGameObject<EnemySlashEffect>();

	if (m_frame >= 20) {
		m_isHit = false;
		effect->SetDestroy();
		SetDestroy();
		return;
	}

	Enemy* enmey = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Enemy>();
	XMFLOAT3 enmeypos = enmey->GetComponent<Transform>()->GetPos();
	XMFLOAT3 enmeyrot = enmey->GetComponent<Transform>()->GetRot();

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();

	pos.x = enmeypos.x + 3.0f * cosf(enmeyrot.y - (XM_PI / 2));
	pos.z = enmeypos.z + 3.0f * sinf(enmeyrot.y + (XM_PI / 2));
	pos.y = enmeypos.y + 2.0f;

	++m_frame;

	SlashCollision();


	GetComponent<Transform3DComponent>()->SetPos(pos);
	GetComponent<Transform3DComponent>()->SetRot(Add(m_Rot, enmeyrot));
	GetComponent<CapsuleColiderComponent>()->SetPos(GetComponent<Transform3DComponent>()->GetPos());
	GetComponent<CapsuleColiderComponent>()->SetRot(GetComponent<Transform3DComponent>()->GetRot());
	effect->GetComponent<Transform2DComponent>()->SetPos(pos);
	effect->GetComponent<Transform2DComponent>()->SetRot(GetComponent<Transform3DComponent>()->GetRot());

	for (auto component : m_ComponentList)
	{
		component->Update();
	}

}

void EnemySlash::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}

void EnemySlash::SlashCollision()
{
	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();

	if (std::get<0>(GetColider()->GetCollision()))
	{
		std::list<GameObject*> objectList = std::get<2>(GetComponent<Colider>()->GetCollision());
		for (auto onCollisionObject : objectList)
		{
			if (onCollisionObject->GetObjectType() == OBJ_TYPE::PLAYER)
			{
				if (!m_isHit)
				{
					onCollisionObject->SetLife(onCollisionObject->GetLife() - 1);
					m_isHit = true;
				}
			}
		}
		GetComponent<Transform3DComponent>()->SetPos(pos);
	}
}
