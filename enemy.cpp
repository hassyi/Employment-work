#include "enemy.h"
#include "modelRenderer.h"
#include "transform3DComponent.h"

void Enemy::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\player.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");


}

void Enemy::Uninit()
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

void Enemy::Update()
{

}

void Enemy::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}

}
