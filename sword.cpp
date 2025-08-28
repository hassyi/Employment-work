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
#include "capsuleColiderComponent.h"

void Sword::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\sword3.obj");
	GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	AddComponent<CapsuleColiderComponent>();
	GetColider()->SetScale(XMFLOAT3(0.5f, 1.0f, 0.5f));

	//Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	//Transform3DAnimationComponent* playerTrans = player->GetComponent<Transform3DAnimationComponent>();
	//GetComponent<Transform3DComponent>()->SetPos(playerTrans->GetPos());
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
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	Transform3DAnimationComponent* playerTrans = player->GetComponent<Transform3DAnimationComponent>();
	XMFLOAT3 playerrot = playerTrans->GetRot();

	XMMATRIX rightHand = playerTrans->GetModel()->GetRightHandMatrix();
	Transform3DComponent* trans = GetComponent<Transform3DComponent>();

	XMFLOAT3 rot = { playerrot.x, playerrot.y ,playerrot.z };
	trans->SetRot(rot);

	trans->SetPos(playerTrans->GetPos());
	trans->SetLocalMatrix(rightHand);

	for (auto component : m_ComponentList)
	{
		component->Update();
	}
}

void Sword::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}

	if (Scene::GetInstance()->GetScene<Game>()->GetIsDrawImGui()) {
		DrawImGui();
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

void Sword::DrawImGui()
{
	XMFLOAT3 transPos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 transRot = GetComponent<Transform3DComponent>()->GetRot();
	XMFLOAT3 coliderPos = GetColider()->GetPos();
	XMFLOAT3 coliderRot = GetColider()->GetRot();

	{
		ImGui::Begin("Sword");

		ImGui::Text("SwordPos : x = %.1f, y = %.1f, z = %.1f", transPos.x, transPos.y, transPos.z);
		ImGui::Text("SwordRot : x = %.1f, y = %.1f, z = %.1f", transRot.x, transRot.y, transRot.z);
		ImGui::Text("ColiderPos : x = %.1f, y = %.1f, z = %.1f", coliderPos.x, coliderPos.y, coliderPos.z);

		ImGui::End();
	}
}
