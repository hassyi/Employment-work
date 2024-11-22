#include "boxColiderComponent.h"
#include "transform.h"

void BoxColiderComponent::Init()
{
	m_Pos = GetGameObject()->GetComponent<Transform>()->GetPos();
	m_Scale = GetGameObject()->GetComponent<Transform>()->GetScale();
	m_Rot = GetGameObject()->GetComponent<Transform>()->GetRot();
}

void BoxColiderComponent::Uninit()
{

}

void BoxColiderComponent::Update()
{
	m_Pos = GetGameObject()->GetComponent<Transform>()->GetPos();
	m_Rot = GetGameObject()->GetComponent<Transform>()->GetRot();
}

void BoxColiderComponent::Draw()
{

}

