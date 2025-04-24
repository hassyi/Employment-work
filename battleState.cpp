#include "battleState.h"
#include "behaviorSequence.h"
#include "behaviorSelector.h"
#include "behaviorMove.h"
#include "behaviorPunch.h"
#include "behaviorGun.h"
#include "behaviorDodge.h"

void BattleState::Init()
{
	m_RootNode = new BehaviorSequence(m_Enemy);
	m_RootNode->AddChild(new BehaviorMove(m_Enemy));
	m_RootNode->AddChild(new BehaviorSequence(m_Enemy));
	m_RootNode->GetChildNum()[1]->AddChild(new BehaviorPunch(m_Enemy));
	m_RootNode->GetChildNum()[1]->AddChild(new BehaviorDodge(m_Enemy));
	m_RootNode->GetChildNum()[1]->AddChild(new BehaviorGun(m_Enemy));
}

void BattleState::Uninit()
{
	m_RootNode->Uninit();
	delete m_RootNode;
}

void BattleState::Update()
{
	m_RootNode->Update();

	
}
