#include "bullet.h"
#include "modelRenderer.h"
#include "scene.h"
#include "manager.h"
#include "player.h"
#include "explosion.h"
#include "collision.h"


void Bullet::Init()
{
	SetPos(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));

	m_Component = new ModelRenderer(this);
	((ModelRenderer*)m_Component)->Load("asset\\model\\bullet.obj");

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
}

void Bullet::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	XMFLOAT3 dir = player->GetDir();
	Collision collision;
	SetPosX(GetPos().x + dir.x);
	SetPosZ(GetPos().z + dir.z);
	++m_frame;

	if (m_frame >= 60){
		Explosion* explosion = scene->AddGameObject<Explosion>(1);
		explosion->SetPos(GetPos());
		SetDestroy();
		return;
	}

	//敵との衝突判定
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>();

	for (Enemy* enemy : enemyList) {
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

		if (collision.CollisionBS(GetPos(), enemyPos, 2.0f)) {
			Explosion* explosion = scene->AddGameObject<Explosion>(1);
			explosion->SetPos(GetPos());
			SetDestroy();
			enemy->SetDestroy();
		}
	}
}

void Bullet::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRot().x, GetRot().y, GetRot().z);
	trans = XMMatrixTranslation(GetPos().x, GetPos().y, GetPos().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	m_Component->Draw();
}
