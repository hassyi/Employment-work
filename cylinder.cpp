#include "cylinder.h"
#include "modelRenderer.h"
#include "scene.h"
#include "manager.h"
#include "collision.h"
#include "transform3DComponent.h"
#include "cylinderColiderComponent.h"

void Cylinder::Init()
{	
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\cylinder.obj");
	AddComponent<CylinderColiderComponent>();

	m_ObjType = OBJ_TYPE::CYLINDER;
}

void Cylinder::Uninit()
{

	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Cylinder::Update()
{
	for (auto component : m_ComponentList)
	{
		component->Update();
	}
}

void Cylinder::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}
