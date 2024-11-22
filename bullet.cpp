#include "bullet.h"
#include "modelRenderer.h"
#include "manager.h"
#include "player.h"
#include "explosion.h"
#include "collision.h"
#include "transform3DComponent.h"
#include "game.h"
#include "enemy.h"

void Bullet::Init()
{
	m_Component = new ModelRenderer(this);
	((ModelRenderer*)m_Component)->Load("asset\\model\\bullet.obj");

	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\bullet.obj");
	GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void Bullet::Uninit()
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

void Bullet::Update()
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	XMFLOAT3 dir = player->GetDir();
	Collision collision;

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();

	pos.x += dir.x;
	pos.z += dir.z;

	++m_frame;

	if (m_frame >= 60){
		Explosion* explosion = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Explosion>();
		explosion->SetPos(pos);
		SetDestroy();
		return;
	}

	//敵との衝突判定
	std::vector<Enemy*> enemyList = Scene::GetInstance()->GetScene<Game>()->GetGameObjects<Enemy>();;

	for (Enemy* enemy : enemyList) 
	{
		XMFLOAT3 enemyPos = enemy->GetPos();
		XMFLOAT3 enemyScale = enemy->GetScale();
		//XMFLOAT3 direction;
		//direction.x = enemyPos.x - GetPos().x;
		//direction.y = enemyPos.y - GetPos().y;
		//direction.z = enemyPos.z - GetPos().z;

		//float length;
		//length = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

		//if (length < 2.0f) {
		//	Explosion* explosion = scene->AddGameObject<Explosion>(1);
		//	explosion->SetPos(GetPos());
		//	SetDestroy();
		//	enemy->SetDestroy();
		//}

		if (collision.CollisionBS(pos, enemyPos, 2.0f)) {
			Explosion* explosion = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Explosion>();
			explosion->SetPos(pos);
			SetDestroy();
			enemy->SetDestroy();
		}
	}

	GetComponent<Transform3DComponent>()->SetPos(pos);
}

void Bullet::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}
}
