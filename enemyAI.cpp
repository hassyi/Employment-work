#include "enemyAI.h"
#include "behaviorSequence.h"
#include "behaviorIdle.h"
#include "behaviorMove.h"
#include "behaviorSearch.h"
#include "behaviorSelector.h"
#include "behaviorPunch.h"
#include "behaviorGun.h"

void EnemyAI::SetNode(Enemy* enemy)
{
	m_BehaviorRoot = new BehaviorSequence(enemy);
	m_BehaviorRoot->AddChild(new BehaviorIdle(enemy));
	m_BehaviorRoot->AddChild(new BehaviorSearch(enemy));
	m_BehaviorRoot->AddChild(new BehaviorMove(enemy));
	m_BehaviorRoot->AddChild(new BehaviorSelector(enemy));
	m_BehaviorRoot->GetChildNum()[3]->AddChild(new BehaviorPunch(enemy));
	m_BehaviorRoot->GetChildNum()[3]->AddChild(new BehaviorGun(enemy));
}

void EnemyAI::Init()
{
}

void EnemyAI::Uninit()
{
	m_BehaviorRoot->Uninit();
	delete m_BehaviorRoot;
}

void EnemyAI::Update()
{
	m_BehaviorRoot->Update();
}

void EnemyAI::Draw()
{
	DrawImGui();
}

void EnemyAI::DrawImGui()
{

	{
		ImGui::Begin("Behavior");

		ImGui::Text("This is some useful text.");
		ImGui::Text("BehaviorRoot : %s", m_BehaviorRoot->GetBehaviorName());

		ImGui::End();
	}

}
