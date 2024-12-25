#include "enemy.h"
#include "scene.h"
#include "game.h"
#include "modelRenderer.h"
#include "transform3DComponent.h"
#include "transform3DAnimaitonComponent.h"
#include "boxColiderComponent.h"
#include "enemyBullet.h"
#include "meshField.h"
#include <random>
#include "player.h"
#include "sphereShadow.h"
#include <iostream>
#include <algorithm>

void Enemy::Init()
{
	AddComponent<Transform3DAnimationComponent>()->AddModelData("asset\\model\\Maw J Laygo.fbx", this);
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\EnemyIdle.fbx", "Idle");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\EnemyWalking.fbx", "Walk");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\EnemyAttack.fbx", "Attack");
	GetComponent<Transform3DAnimationComponent>()->SetInitAnimationState("Idle");

	AddComponent<Transform>();
	AddComponent<BoxColiderComponent>();
	GetComponent<Transform>()->SetScale(XMFLOAT3(0.02f, 0.02f, 0.02f));
	AddComponent<SphereShadow>()->SetScale(XMFLOAT3(1.5f, 1.5f, 1.5f));

	SetLife(10);

	m_ObjType = OBJ_TYPE::ENEMY;

	for (auto component : m_ComponentList)
	{
		component->Init();
	}

	GetComponent<BoxColiderComponent>()->SetScale(XMFLOAT3(1.0f, 2.0f, 1.0f));
}

void Enemy::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Enemy::Update()
{
	PlayerFollow();
	EnemyCollision();
	//m_CreateBulletFrame++;
	if (m_CreateBulletFrame > 120)
	{
		Scene::GetInstance()->GetScene<Game>()->AddGameObject<EnemyBullet>(1)->GetComponent<Transform3DComponent>()->SetPos(GetComponent<Transform3DComponent>()->GetPos());
		m_CreateBulletFrame = 0;
	}

	XMFLOAT3 pos = GetComponent<Transform>()->GetPos();
	float gravity = -10.0f;
	pos.y += gravity;

	if (pos.y < m_GroundHeight) {
		pos.y = m_GroundHeight;
		gravity = 0.0f;
	}
	GetComponent<Transform>()->SetPosY(pos.y);
	GetComponent<SphereShadow>()->SetPos(pos);
	
	if (GetLife() <= 0)
	{
		SetDestroy();
		return;
	}
	
	for (auto component : m_ComponentList)
	{
		component->Update();
	}

}

void Enemy::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
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

void  Enemy::PlayerFollow()
{
	XMFLOAT3 playerpos = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPos();

	XMFLOAT3 pos = GetComponent<Transform>()->GetPos();
	XMFLOAT3 rot = GetComponent<Transform>()->GetRot();
	XMFLOAT3 direction{};
	float length;

	direction.x = playerpos.x - pos.x;
	direction.y = playerpos.y - pos.y;
	direction.z = playerpos.z - pos.z;

	length = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	direction.x /= length;
	direction.z /= length;

	if (length >= 3.5f && !m_isAttack) {
		m_Speed = 0.02f;
		GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Walk");
	}
	else {
		m_Speed = 0.0f;
		EnemyAttack();
	}
	pos.x += direction.x * m_Speed;
	pos.z += direction.z * m_Speed;

	GetComponent<Transform>()->SetPos(pos);

	//プレイヤーまでのベクトル
	XMFLOAT3 vec = XMFLOAT3(playerpos.x - pos.x, playerpos.y - pos.y, playerpos.z - pos.z);
	float len = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x /= len;
	vec.y /= len;
	vec.z /= len;
	XMFLOAT3 FrontVec = GetComponent<Transform>()->GetForward();
	XMFLOAT3 RightVec = GetComponent<Transform>()->GetRight();

	{
		float dot = vec.x * FrontVec.x + vec.y + FrontVec.y + vec.z * FrontVec.z;

		float vec1len = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		float vec2len = sqrtf(FrontVec.x * FrontVec.x + FrontVec.y * FrontVec.y + FrontVec.z * FrontVec.z);

		//ベクトルの大きさが0だったら処理を中止
		if (vec1len == 0 || vec2len == 0)
		{
			return;
		}

		// cosθを計算
		float cosTheta = dot / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acosf(cosTheta);
		float angleDegrees = angleRadians * 180.0f / XM_PI;

		//ほぼ正面を向いている
		if (angleDegrees < 5.0f)
		{
			return;
		}

	}

	{
		float dot = vec.x * RightVec.x + vec.y * RightVec.y + vec.z * RightVec.z;

		float vec1len = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		float vec2len = sqrtf(RightVec.x * RightVec.x + RightVec.y * RightVec.y + RightVec.z * RightVec.z);

		//ベクトルの大きさが0だったら処理を中止
		if (vec1len == 0 || vec2len == 0)
		{
			return;
		}

		// cosθを計算
		float cosTheta = dot / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acosf(cosTheta);
		float angleDegrees = angleRadians * 180.0f / XM_PI;

		if (angleDegrees < 90.0f)
		{
			XMFLOAT3 rot = GetComponent<Transform>()->GetRot();
			rot.y += 0.02f;
			GetComponent<Transform>()->SetRot(rot);
		}
		else
		{
			XMFLOAT3 rot = GetComponent<Transform>()->GetRot();
			rot.y -= 0.02f;
			GetComponent<Transform>()->SetRot(rot);
		}

	}

}

void Enemy::EnemyAttack()
{
	if (m_isAttack) m_AttackFrame--;
	if (!m_isAttack && m_Speed == 0.0f)
	{
		GetComponent<Transform3DAnimationComponent>()->SetAnimationFrame(0);
		m_isAttack = true;
		m_AttackFrame = 80;
		GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Attack");
	}

	if (m_AttackFrame <= 0) {
		m_isAttack = false;
		m_AttackFrame = 0;
	}
}

