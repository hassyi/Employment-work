#include "enemy.h"
#include "scene.h"
#include "game.h"
#include "modelRenderer.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "enemyBullet.h"
#include "meshField.h"

void Enemy::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\player.obj");
	AddComponent<BoxColiderComponent>();

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_ObjType = OBJ_TYPE::ENEMY;

	for (auto component : m_ComponentList)
	{
		component->Init();
	}

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
	SetPos(GetComponent<Transform3DComponent>()->GetPos());
	EnemyCollision();
	m_CreateBulletFrame++;
	if (m_CreateBulletFrame > 120)
	{
		Scene::GetInstance()->GetScene<Game>()->AddGameObject<EnemyBullet>(1)->GetComponent<Transform3DComponent>()->SetPos(GetPos());
		m_CreateBulletFrame = 0;
	}

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
	if (pos.y < m_GroundHeight)
	{
		pos.y = m_GroundHeight;

	}
	GetComponent<Transform3DComponent>()->SetPosY(pos.y);

	for (auto component : m_ComponentList)
	{
		component->Update();
	}
}

void Enemy::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}

}

void Enemy::EnemyCollision()
{

	MeshField* meshField = Scene::GetInstance()->GetScene<Game>()->GetGameObject<MeshField>();
	//地面の高さ
	m_GroundHeight = meshField->GetHeight(GetComponent<Transform3DComponent>()->GetPos());
}
