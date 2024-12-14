#include "box.h"
#include "modelRenderer.h"
#include "scene.h"
#include "manager.h"
#include "collision.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"

void Box::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\box.obj");
	AddComponent<BoxColiderComponent>();

	m_ObjType = OBJ_TYPE::BOX;

	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void Box::Uninit()
{

	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Box::Update()
{
	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 scale = GetComponent<Transform3DComponent>()->GetScale();

	GetComponent<BoxColiderComponent>()->SetPos(pos);
	GetComponent<BoxColiderComponent>()->SetScale(XMFLOAT3(scale.x, scale.y / 2, scale.z));

	for (auto component : m_ComponentList)
	{
		component->Update();
	}
}

void Box::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}
