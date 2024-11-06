#include "predation.h"
#include "modelRenderer.h"
#include "manager.h"
#include "scene.h"
#include "enemy.h"
#include "collision.h"

void Predation::Init()
{
	m_Component = new ModelRenderer(this);
	((ModelRenderer*)m_Component)->Load("asset\\model\\player.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

}

void Predation::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Predation::Update()
{
	if (m_use) {
		m_frame++;
		if (m_frame >= 30) {
			m_use = false;
			m_frame = 0;
		}
		PredationCollision();
	}		

}

void Predation::Draw()
{
	if (m_use)
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
}

void Predation::PredationCollision()
{
	Scene* scene = Manager::GetScene();
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>();
	Collision collision;

	for (auto enemy : enemyList)
	{
		if (collision.CollisionBB(GetPos(), enemy->GetPos(), GetScale(), enemy->GetScale())) {
			enemy->SetDestroy();
			SetBuff(true);
		}


	}
}

