#include "predation.h"
#include "modelRenderer.h"
#include "manager.h"
#include "scene.h"
#include "enemy.h"
#include "collision.h"
#include "transform3DComponent.h"
#include "game.h"

void Predation::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\player.obj");
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
		if (m_frame >= 30) {
			m_use = false;
			m_frame = 0;
		}
		PredationCollision();
	}		

}

void Predation::Draw()
{
	if (m_use)
	{
		for (auto component : m_ComponentList)
		{
			component->Draw();
		}
	}
}

void Predation::PredationCollision()
{
	std::vector<Enemy*> enemyList = Scene::GetInstance()->GetScene<Game>()->GetGameObjects<Enemy>();;
	Collision collision;

	for (auto enemy : enemyList)
	{
		if (collision.CollisionBB(GetPos(), enemy->GetPos(), GetScale(), enemy->GetScale())) {
			enemy->SetDestroy();
			SetBuff(true);
		}


	}
}

