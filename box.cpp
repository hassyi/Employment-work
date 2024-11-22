#include "box.h"
#include "modelRenderer.h"
#include "scene.h"
#include "manager.h"
#include "collision.h"
#include "transform3DComponent.h"

void Box::Init()
{
	SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));

	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\box.obj");


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void Box::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Box::Update()
{

}

void Box::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}
