#include "enemy.h"
#include "scene.h"
#include "game.h"
#include "modelRenderer.h"
#include "transform3DComponent.h"
#include "transform3DAnimaitonComponent.h"
#include "boxColiderComponent.h"
#include "capsuleColiderComponent.h"
#include "meshField.h"
#include <random>
#include "player.h"
#include "sphereShadow.h"
#include <iostream>
#include <algorithm>
#include "enemyAIState.h"
#include "enemyAI.h"

void Enemy::Init()
{
	AddComponent<Transform3DAnimationComponent>()->AddModelData("asset\\model\\Maw J Laygo.fbx", this);
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\EnemyIdle.fbx", "Idle");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\EnemyWalking.fbx", "Walk");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\EnemyAttack.fbx", "Attack");
	GetComponent<Transform3DAnimationComponent>()->SetInitAnimationState("Idle");

	AddComponent<Transform>();
	//AddComponent<BoxColiderComponent>();
	AddComponent<CapsuleColiderComponent>();
	//AddComponent<EnemyAIState>();
	AddComponent<EnemyAI>()->SetNode(this);
	GetComponent<Transform>()->SetScale(XMFLOAT3(0.02f, 0.02f, 0.02f));
	AddComponent<SphereShadow>()->SetScale(XMFLOAT3(1.5f, 1.5f, 1.5f));

	SetLife(10.0f);

	m_ObjType = OBJ_TYPE::ENEMY;

	for (auto component : m_ComponentList)
	{
		component->Init();
	}

	//GetComponent<BoxColiderComponent>()->SetScale(XMFLOAT3(1.0f, 2.0f, 1.0f));
	GetComponent<CapsuleColiderComponent>()->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
}

void Enemy::Uninit()
{
	//GetComponent<EnemyAIState>()->Uninit();

	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Enemy::Update()
{

	EnemyCollision();

	//GetComponent<EnemyAIState>()->Update(this);
	GetComponent<EnemyAI>()->Update();

	XMFLOAT3 pos = GetComponent<Transform>()->GetPos();

	if (m_IsGravity) 
	{
		m_Gravity = -10.0f;
		pos.y += m_Gravity;
	}

	if (pos.y < m_GroundHeight) {
		pos.y = m_GroundHeight;
		m_IsGravity = false;
		m_Gravity = 0.0f;
	}
	else {
		m_IsGravity = true;
	}
	GetComponent<Transform>()->SetPosY(pos.y);
	GetComponent<SphereShadow>()->SetPos(pos);
	

	
	for (auto component : m_ComponentList)
	{
		component->Update();
	}

	if (GetLife() <= 0.0f)
	{
		SetDestroy();
		return;
	}
}

void Enemy::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
	DrawImGui();
}

void Enemy::EnemyCollision()
{

	MeshField* meshField = Scene::GetInstance()->GetScene<Game>()->GetGameObject<MeshField>();
	//地面の高さ
	m_GroundHeight = meshField->GetHeight(GetComponent<Transform>()->GetPos());

	XMFLOAT3 pos = GetComponent<Transform>()->GetPos();
	XMFLOAT3 scale = GetComponent<Transform>()->GetScale();
	XMFLOAT3 vel = GetComponent<Transform>()->GetVel();
	GetColider()->SetPos(pos);
	XMFLOAT3 coliderPos = GetColider()->GetPos();
	XMFLOAT3 coliderScale = GetColider()->GetScale();
	//地面の高さ
	m_GroundHeight = meshField->GetHeight(pos);

	GetColider()->GetCollision();

	if (std::get<0>(GetColider()->GetCollision()) == true)
	{
		std::list<GameObject*> objectList = std::get<2>(GetComponent<Colider>()->GetCollision());
		for (auto onCollisionObject : objectList)
		{
			if (onCollisionObject->GetObjectType() == OBJ_TYPE::BOX)
			{
				float boxposy = onCollisionObject->GetColider()->GetPos().y + (onCollisionObject->GetColider()->GetScale().y * 2);
				if (coliderPos.y - coliderScale.y >= boxposy)
				{
					m_GroundHeight = boxposy + onCollisionObject->GetColider()->GetScale().y + 1.0f;
				}
				else
				{
					float geta = 1.0f;
					float boxPosX = onCollisionObject->GetColider()->GetPos().x;
					float boxPosZ = onCollisionObject->GetColider()->GetPos().z;
					float boxScaleX = onCollisionObject->GetColider()->GetScale().x;
					float boxScaleZ = onCollisionObject->GetColider()->GetScale().z;
					float disx = coliderPos.x - boxPosX;
					float disz = coliderPos.z - boxPosZ;

					//計算しやすいように距離の値を＋にする
					if (disx < 0) {
						disx *= -1;
					}
					if (disz < 0) {
						disz *= -1;
					}

					//当たったオブジェクトの角度を調べる
					float rotbb = atan(disx / disz);

					//当たったオブジェクトの中心からの角度
					float rot = atan(boxScaleX / boxScaleZ);

					if (rotbb >= rot)
					{
						if (0.0f > coliderPos.x - boxPosX) {
							coliderPos.x = (boxPosX - boxScaleX - geta) - scale.x / 2;
						}
						else if (0.0f < coliderPos.x - boxPosX) {
							coliderPos.x = (boxPosX + boxScaleX + geta) + scale.x / 2;
						}
					}
					else if (rotbb <= rot)
					{
						if (0.0f > coliderPos.z - boxPosZ) {
							coliderPos.z = (boxPosZ - boxScaleZ - geta) - scale.z / 2;
						}
						else if (0.0f < coliderPos.z - boxPosZ) {
							coliderPos.z = (boxPosZ + boxScaleZ + geta) + scale.z / 2;
						}
					}
					GetComponent<Transform>()->SetPos(coliderPos);
					GetColider()->SetPos(coliderPos);
				}
			}
		}
	}
	else
	{
		GetComponent<Transform>()->SetPos(pos);
		GetComponent<Transform>()->SetVel(vel);
		GetColider()->SetPos(pos);
	}


}

void Enemy::DrawImGui()
{
	XMFLOAT3 transPos = GetComponent<Transform>()->GetPos();
	XMFLOAT3 transRot = GetComponent<Transform>()->GetRot();
	XMFLOAT3 coliderPos = GetColider()->GetPos();
	XMFLOAT3 coliderRot = GetColider()->GetRot();

	{
		ImGui::Begin("Enemy");

		ImGui::Text("This is some useful text.");
		ImGui::Text("EnemyPos : x = %.1f, y = %.1f, z = %.1f", transPos.x, transPos.y, transPos.z);
		ImGui::Text("EnemyRot : x = %.1f, y = %.1f, z = %.1f", transRot.x, transRot.y, transRot.z);
		//ImGui::Text("Life : %d", GetLife());

		ImGui::End();
	}


}
