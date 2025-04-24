#include "sword.h"
#include "modelRenderer.h"
#include "manager.h"
#include "player.h"
#include "explosion.h"
#include "collision.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "transform3DAnimaitonComponent.h"
#include "game.h"
#include "enemy.h"
#include "sphereColiderComponent.h"

void Sword::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\box.obj");
	GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(1.0f / 0.5f, 1.0f / 0.5f, 1.0f / 0.5f));
	//AddComponent<SphereColiderComponent>();

	//m_ObjType = OBJ_TYPE::BULLET;

	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void Sword::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Sword::Update()
{
	for (auto component : m_ComponentList)
	{
		component->Update();
	}
}

void Sword::Draw()
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	Transform* playerTrans = player->GetComponent<Transform>();
	XMFLOAT3 playerrot = playerTrans->GetRot();

	XMMATRIX rightHand = player->GetComponent<Transform3DAnimationComponent>()->GetModel()->GetRightHandMatrix();
	Transform3DComponent* trans = GetComponent<Transform3DComponent>();

	XMFLOAT3 rot = { playerrot.x, playerrot.y ,playerrot.z };
	trans->SetRot(rot);

	trans->SetPos(playerTrans->GetPos());
	trans->SetLocalMatrix(rightHand);

	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}

void Sword::SwordCollision()
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
