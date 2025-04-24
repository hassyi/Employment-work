#include "box.h"
#include "modelRenderer.h"
#include "scene.h"
#include "game.h"
#include "collision.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include <random>
#include "camera.h"

void Box::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\box.obj");
	AddComponent<BoxColiderComponent>();

	m_ObjType = OBJ_TYPE::BOX;

	for (auto component : m_ComponentList)
	{
		component->Init();
	}

	//m_Add = GetComponent<Transform3DComponent>()->GetRot();
	m_Add.x = (rand() % 4) / XM_PI;
	m_Add.y = (rand() % 4) / XM_PI;
	m_Add.z = (rand() % 4) / XM_PI;

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
	XMFLOAT3 rot = GetComponent<Transform3DComponent>()->GetRot();

	rot = m_Add;

	GetComponent<BoxColiderComponent>()->SetPos(pos);
	GetComponent<BoxColiderComponent>()->SetScale(scale);
	GetComponent<BoxColiderComponent>()->SetRot(rot);
	GetComponent<Transform3DComponent>()->SetRot(rot);
	

	for (auto component : m_ComponentList)
	{
		component->Update();
	}
}

void Box::Draw()
{
	if (Scene::GetInstance()->GetScene<Game>()->GetIsDrawImGui()) {
		DrawImGui();
	}

	Camera* camera = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Camera>();
	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 scale = GetComponent<Transform3DComponent>()->GetScale();

	if (camera->CheckView(pos, scale.x) == false) return;

	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}

void Box::DrawImGui()
{
	XMFLOAT3 transPos = GetComponent<Transform>()->GetPos();
	XMFLOAT3 transScale = GetComponent<Transform>()->GetScale();
	XMFLOAT3 transRot = GetComponent<Transform>()->GetRot();
	XMFLOAT3 coliderPos = GetColider()->GetPos();
	XMFLOAT3 coliderScale = GetColider()->GetScale();
	XMFLOAT3 coliderRot = GetColider()->GetRot();

	{
		ImGui::Begin("Box");

		ImGui::Text("BoxPos : x = %.1f, y = %.1f, z = %.1f", transPos.x, transPos.y, transPos.z);
		ImGui::Text("ColiderPos : x = %.1f, y = %.1f, z = %.1f", coliderPos.x, coliderPos.y, coliderPos.z);
		ImGui::Text("BoxScale : x = %.1f, y = %.1f, z = %.1f", transScale.x, transScale.y, transScale.z);
		ImGui::Text("ColiderScale : x = %.1f, y = %.1f, z = %.1f", coliderScale.x, coliderScale.y, coliderScale.z);
		ImGui::Text("BoxRot : x = %.1f, y = %.1f, z = %.1f", transRot.x, transRot.y, transRot.z);

		static float add[3] = { 0.0f,0.0f,0.0f };

		ImGui::SliderFloat3("AddRot", add, 0.0f, 3.0f);
		//SetAdd(XMFLOAT3(add[0], add[1], add[2]));


		ImGui::End();
	}

}
