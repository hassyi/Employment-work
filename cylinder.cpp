#include "cylinder.h"
#include "modelRenderer.h"
#include "scene.h"
#include "manager.h"
#include "collision.h"
#include "transform3DComponent.h"

void Cylinder::Init()
{	
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\cylinder.obj");


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexDirectionalLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexDirectionalLightingPS.cso");

}

void Cylinder::Uninit()
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

void Cylinder::Update()
{

}

void Cylinder::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}
