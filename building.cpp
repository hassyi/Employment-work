#include "building.h"
#include "modelRenderer.h"
#include "scene.h"
#include "game.h"
#include "manager.h"
#include "collision.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "camera.h"

void Building::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\Building01.obj");
	AddComponent<BoxColiderComponent>();

	m_ObjType = OBJ_TYPE::BUILDING;

	for (auto component : m_ComponentList)
	{
		component->Init();
	}


}

void Building::Uninit()
{

	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Building::Update()
{
	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 scale = GetComponent<Transform3DComponent>()->GetScale();
	XMFLOAT3 rot = GetComponent<Transform3DComponent>()->GetRot();

	GetComponent<BoxColiderComponent>()->SetPos(pos);
	GetComponent<BoxColiderComponent>()->SetScale(XMFLOAT3(4.0f, 6.0f, 3.0f));
	GetComponent<BoxColiderComponent>()->SetRot(rot);
	GetComponent<Transform3DComponent>()->SetRot(rot);
	

	for (auto component : m_ComponentList)
	{
		component->Update();
	}
}

void Building::Draw()
{
	Camera* camera = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Camera>();
	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 scale = GetComponent<Transform3DComponent>()->GetScale();

	if (camera->CheckView(pos, scale.x) == false) return;

	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}
