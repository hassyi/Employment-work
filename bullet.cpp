#include "bullet.h"
#include "modelRenderer.h"
#include "manager.h"
#include "player.h"
#include "explosion.h"
#include "collision.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "game.h"
#include "enemy.h"
#include "capsuleColiderComponent.h"

void Bullet::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\bullet2.obj");
	GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	AddComponent<CapsuleColiderComponent>();

	XMFLOAT3 rot = GetComponent<Transform3DComponent>()->GetRot();
	GetComponent<CapsuleColiderComponent>()->SetRot(XMFLOAT3(rot.x + XM_PI / 2, rot.y, rot.z));
	GetComponent<CapsuleColiderComponent>()->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	GetComponent<CapsuleColiderComponent>()->SetSegmentLength(1.0f);

	m_ObjType = OBJ_TYPE::BULLET;

	for (auto component : m_ComponentList)
	{
		component->Init();
	}

}

void Bullet::Uninit()
{

	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Bullet::Update()
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	XMFLOAT3 dir = player->GetDir();
	Collision collision;

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();

	pos.x += dir.x;
	pos.z += dir.z;

	++m_frame;

	BulletCollision();

	if (m_frame >= 60){
		Scene::GetInstance()->GetScene<Game>()->AddGameObject<Explosion>(1)->GetComponent<Transform2DComponent>()->SetPos(pos);
		m_isHit = false;
		SetDestroy();
		return;
	}

	GetComponent<Transform3DComponent>()->SetPos(pos);
	GetComponent<CapsuleColiderComponent>()->SetPos(pos);


	for (auto component : m_ComponentList)
	{
		component->Update();
	}

}

void Bullet::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}

void Bullet::BulletCollision()
{
	if (std::get<0>(GetColider()->GetCollision()))
	{
		std::list<GameObject*> objectList = std::get<2>(GetComponent<Colider>()->GetCollision());
		for (auto onCollisionObject : objectList)
		{
			if (onCollisionObject->GetObjectType() == OBJ_TYPE::ENEMY)
			{
				if (!m_isHit)
				{
					Explosion* explosion = Scene::GetInstance()->GetScene<Game>()->AddGameObject<Explosion>(1);
					explosion->GetComponent<Transform2DComponent>()->SetPos(GetComponent<Transform3DComponent>()->GetPos());
					SetDestroy();
					onCollisionObject->SetLife(onCollisionObject->GetLife() - 1);
					m_isHit = true;
				}
			}
		}
	}
}
