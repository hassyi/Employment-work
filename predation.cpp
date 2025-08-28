#include "predation.h"
#include "modelRenderer.h"
#include "manager.h"
#include "scene.h"
#include "enemy.h"
#include "collision.h"
#include "game.h"
#include "capsuleColiderComponent.h"
#include "player.h"

void Predation::Init()
{
	m_Kiba_ue = AddComponent<Transform3DComponent>();
	m_Kiba_ue->AddModelData("asset\\model\\kiba_ue.obj");
	m_Kiba_sita = AddComponent<Transform3DComponent>();
	m_Kiba_sita->AddModelData("asset\\model\\kiba_sita.obj");

	AddComponent<CapsuleColiderComponent>()->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	GetComponent<CapsuleColiderComponent>()->SetSegmentLength(1.0f);

	m_Kiba_ue->SetRotZ(0.0f);
	m_Kiba_sita->SetRotZ(0.0f);

	m_ObjType = PREDATION;
}

void Predation::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}

}

void Predation::Update()
{
	m_Time += 0.1f;
	float t = std::min(m_Time, 1.0f);

	RotationAnim(t);

	m_Frame++;
	GetColider()->SetPos(m_Kiba_sita->GetPos());
	XMFLOAT3 clliderPos = GetColider()->GetPos();
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();

	clliderPos.x = player->GetDir().x + 1.0f;
	clliderPos.z = player->GetDir().z + 1.0f;

	GetColider()->SetPos(clliderPos);

	PredationCollision();
	for (auto component : m_ComponentList)
	{
		component->Update();
	}
	if (m_Frame >= 30) {
		m_IsHit = false;
		m_Frame = 0;
		SetDestroy();
	}

}

void Predation::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}

void Predation::PredationCollision()
{
	std::list<Enemy*> enemyList = Scene::GetInstance()->GetScene<Game>()->GetGameObjectList<Enemy>();

	XMFLOAT3 pos = m_Kiba_sita->GetPos();
	GetComponent<Transform>()->SetPos(m_Kiba_sita->GetPos());
	

	if (enemyList.size() == 0)return;

	if (std::get<0>(GetColider()->GetCollision()) == true)
	{
		std::list<GameObject*> objectList = std::get<2>(GetComponent<Colider>()->GetCollision());
		for (auto onCollisionObject : objectList)
		{
			if (onCollisionObject->GetObjectType() == OBJ_TYPE::ENEMY)
			{
				if (!m_IsHit)
				{
					Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->SetIsBuff(true);
					onCollisionObject->SetLife(onCollisionObject->GetLife() - 1);
					m_IsHit = true;
				}
			}
		}


	}
}

void Predation::RotationAnim(float t)
{
	XMFLOAT3 rot_ue = m_Kiba_ue->GetRot();
	XMFLOAT3 rot_sita = m_Kiba_sita->GetRot();

	XMFLOAT3 startRot{};
	
	XMFLOAT3 ueKibaEndRot = XMFLOAT3(rot_ue.x, rot_ue.y, m_Kiba_ue_RotTarget);
	XMFLOAT3 sitaKibaEndRot = XMFLOAT3(rot_sita.y, rot_sita.y, m_Kiba_sita_RotTarget);

	XMFLOAT3 lerpRot_ue = rot_ue;
	lerpRot_ue.z = (1.0f - t) * startRot.z + t * ueKibaEndRot.z;

	XMFLOAT3 lerpRot_sita = rot_sita;
	lerpRot_sita.z = (1.0f - t) * startRot.z + t * sitaKibaEndRot.z;

	m_Kiba_ue->SetRot(lerpRot_ue);
	m_Kiba_sita->SetRot(lerpRot_sita);
}

