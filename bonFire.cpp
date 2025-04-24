#include "bonFire.h"
#include "modelRenderer.h"
#include "scene.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "fireParticle.h"
#include "title.h"

void BonFire::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\takibi.obj");
	GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
}

void BonFire::Uninit()
{

	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void BonFire::Update()
{
	FireParticle* fire = Scene::GetInstance()->GetScene<Title>()->GetGameObject<FireParticle>();
	fire->GetComponent<Transform2DComponent>()->SetPos(GetComponent<Transform3DComponent>()->GetPos());

	for (auto component : m_ComponentList)
	{
		component->Update();
	}
}

void BonFire::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}
