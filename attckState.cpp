#include "attckState.h"
#include "behaviorSelector.h"
#include "behaviorPunch.h"
#include "behaviorGun.h"


void AttackState::Init()
{
	m_RootNode = new BehaviorSelector(m_Enemy);
	m_RootNode->AddChild(new BehaviorPunch(m_Enemy));
	m_RootNode->AddChild(new BehaviorGun(m_Enemy));
}

void AttackState::Uninit()
{
	m_RootNode->Uninit();
	delete m_RootNode;
}

void AttackState::Update()
{
	m_RootNode->Update();
}
