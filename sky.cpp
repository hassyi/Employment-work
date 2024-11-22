#include "sky.h"
#include "transform3DComponent.h"

void Sky::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\player.obj");
	GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(0.0f, 10.0f, 0.0f));
	GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(1000.0f, 100.0f, 1000.0f));
	GetComponent<Transform3DComponent>()->SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));

}

void Sky::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Sky::Update()
{

}

void Sky::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}
