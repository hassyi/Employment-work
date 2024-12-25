#include "sphereShadow.h"
#include "animationModel.h"
#include "input.h"
#include "camera.h"
#include "scene.h"
#include "manager.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "game.h"
#include "buffParticle.h"

void SphereShadow::Init()
{
	m_Component = new ModelRenderer;
	((ModelRenderer*)m_Component)->Load("asset\\model\\sphere_smooth.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void SphereShadow::Uninit()
{
	m_Component->Uninit();
	delete m_Component;


	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void SphereShadow::Update()
{
	
}

void SphereShadow::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	trans = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	//カラーバッファマスク有効
	Renderer::SetBlendMaskEnable(true);

	//ステンシル書込有効
	Renderer::SetStencilEnable(true);

	//カリングを無効
	Renderer::SetCullEnable(false);

	m_Component->Draw();

	//カラーバッファマスク無効
	Renderer::SetBlendMaskEnable(false);

	//ステンシル書込無効
	Renderer::SetDepthEnable(true);

	//カリング有効
	Renderer::SetCullEnable(true);

}
