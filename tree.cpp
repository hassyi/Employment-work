#include "tree.h"
#include "modelRenderer.h"
#include "manager.h"
#include "camera.h"
#include "game.h"
#include "transform2DComponent.h"

void Tree::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-1.0f, 2.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(1.0f, 2.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	AddComponent<Transform2DComponent>()->AddTextureData(L"asset\\texture\\tree_2.png");
	GetComponent<Transform2DComponent>()->SetVertex(vertex);
	GetComponent<Transform2DComponent>()->SetBillBoard(true);
	GetComponent<Transform2DComponent>()->SetZBuff(true);
	GetComponent<Transform2DComponent>()->SetScale(XMFLOAT3(3.0f, 4.0f, 3.0f));
	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void Tree::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Tree::Update()
{

}

void Tree::Draw()
{

	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}
