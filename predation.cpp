#include "predation.h"
#include "modelRenderer.h"
#include "manager.h"
#include "scene.h"
#include "enemy.h"
#include "collision.h"
#include "transform3DComponent.h"
#include "game.h"
#include "boxColiderComponent.h"

void Predation::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\player.obj");
	AddComponent<BoxColiderComponent>()->SetScale(XMFLOAT3(0.6f, 1.0f, 0.6f));
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
	if (m_use) {
		m_frame++;
		GetColider()->SetPos(GetComponent<Transform3DComponent>()->GetPos());
		PredationCollision();
		for (auto component : m_ComponentList)
		{
			component->Update();
		}
		if (m_frame >= 30) {
			m_use = false;
			m_isHit = false;
			m_frame = 0;
		}
	}		

}

void Predation::Draw()
{
	if (m_use)
	{
		XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
		for (auto component : m_ComponentList)
		{
			component->Draw();
		}
	}
}

void Predation::PredationCollision()
{
	std::list<Enemy*> enemyList = Scene::GetInstance()->GetScene<Game>()->GetGameObjectList<Enemy>();

	if (enemyList.size() == 0)return;

	if (std::get<0>(GetColider()->GetCollision()) == true)
	{
		std::list<GameObject*> objectList = std::get<2>(GetComponent<Colider>()->GetCollision());
		for (auto onCollisionObject : objectList)
		{
			if (onCollisionObject->GetObjectType() == OBJ_TYPE::ENEMY)
			{
				if (!m_isHit)
				{
					SetBuff(true);
					onCollisionObject->SetLife(onCollisionObject->GetLife() - 1);
					m_isHit = true;
				}
			}
		}
	}
}

