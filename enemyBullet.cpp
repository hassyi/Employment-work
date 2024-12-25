#include "enemyBullet.h"
#include "modelRenderer.h"
#include "manager.h"
#include "player.h"
#include "explosion.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "game.h"
#include "sphereColiderComponent.h"

void EnemyBullet::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\bullet.obj");
	GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	AddComponent<SphereColiderComponent>();

	m_ObjType = OBJ_TYPE::BULLET_ENEMY;

	m_PlayerPos = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->GetPos();
	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void EnemyBullet::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void EnemyBullet::Update()
{
	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();

	XMFLOAT3 direction;
	direction.x = m_PlayerPos.x - pos.x;
	direction.y = m_PlayerPos.y - pos.y;
	direction.z = m_PlayerPos.z - pos.z;

	XMFLOAT3 dir = direction;
	float length = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);		//Normalize
	dir.x /= length;
	dir.y /= length;
	dir.z /= length;

	float speed = 0.75f;

	pos.x += dir.x * speed;
	pos.y += dir.y * speed;
	pos.z += dir.z * speed;

	EnemyBulletCollision();
	++m_Frame;

	if (m_Frame >= 40 || length <= 0.1f) {
		Scene::GetInstance()->GetScene<Game>()->AddGameObject<Explosion>(1)->GetComponent<Transform2DComponent>()->SetPos(pos);
		SetDestroy();
		return;
	}

	GetComponent<Transform3DComponent>()->SetPos(pos);

	for (auto component : m_ComponentList)
	{
		component->Update();
	}
}

void EnemyBullet::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}

void EnemyBullet::EnemyBulletCollision()
{
	if (std::get<0>(GetColider()->GetCollision()))
	{
		std::list<GameObject*> objectList = std::get<2>(GetComponent<Colider>()->GetCollision());
		for (auto onCollisionObject : objectList)
		{
			if (onCollisionObject->GetObjectType() == OBJ_TYPE::PLAYER)
			{
				Explosion* explosion = Scene::GetInstance()->GetScene<Game>()->AddGameObject<Explosion>(1);
				explosion->GetComponent<Transform2DComponent>()->SetPos(GetComponent<Transform3DComponent>()->GetPos());
				SetDestroy();
			}
		}
	}
}

