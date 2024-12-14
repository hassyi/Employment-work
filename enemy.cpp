#include "enemy.h"
#include "scene.h"
#include "game.h"
#include "modelRenderer.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "enemyBullet.h"
#include "meshField.h"
#include <random>
#include "player.h"

void Enemy::Init()
{

	m_Component = new AnimationModel(this);
	((AnimationModel*)m_Component)->Load("asset\\model\\hyena.fbx");

	AddComponent<Transform3DComponent>();
	AddComponent<BoxColiderComponent>();
	GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

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
	m_Component->Uninit();
	delete m_Component;

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
	EnemyCollision();
	PlayerFollow();
	m_CreateBulletFrame++;
	if (m_CreateBulletFrame > 120)
	{
		Scene::GetInstance()->GetScene<Game>()->AddGameObject<EnemyBullet>(1)->GetComponent<Transform3DComponent>()->SetPos(GetPos());
		m_CreateBulletFrame = 0;
	}

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
	float gravity = -10.0f;
	pos.y += gravity;

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
	XMFLOAT3 position = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 rotation = GetComponent<Transform3DComponent>()->GetRot();
	XMFLOAT3 size = GetComponent<Transform3DComponent>()->GetScale();


	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(size.x, size.y, size.z);
	rot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	trans = XMMatrixTranslation(position.x, position.y, position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	m_Component->Draw();

	//for (auto component : m_ComponentList)
	//{
	//	component->Draw();
	//}

}

void Enemy::EnemyCollision()
{

	MeshField* meshField = Scene::GetInstance()->GetScene<Game>()->GetGameObject<MeshField>();
	//地面の高さ
	m_GroundHeight = meshField->GetHeight(GetComponent<Transform3DComponent>()->GetPos());
}

void  Enemy::PlayerFollow()
{
	XMFLOAT3 playerpos = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->GetComponent<Transform3DComponent>()->GetPos();

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 direction;
	float length;
	float speed;

	direction.x = playerpos.x - GetComponent<Transform3DComponent>()->GetPos().x;
	direction.y = playerpos.y - GetComponent<Transform3DComponent>()->GetPos().y;
	direction.z = playerpos.z - GetComponent<Transform3DComponent>()->GetPos().z;

	length = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	direction.x /= length;
	direction.z /= length;
	
	if (length < 5.0f){
		speed = 0.0f;
	}
	else {
		speed = 0.02f;
	}
	pos.x += direction.x * speed;
	pos.z += direction.z * speed;



	GetComponent<Transform3DComponent>()->SetPos(pos);
}

