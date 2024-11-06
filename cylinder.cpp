#include "cylinder.h"
#include "modelRenderer.h"
#include "scene.h"
#include "manager.h"
#include "collision.h"

void Cylinder::Init()
{	
	SetScale(XMFLOAT3(5.0f, 5.0f, 5.0f));
	SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));

	m_Component = new ModelRenderer(this);
	((ModelRenderer*)m_Component)->Load("asset\\model\\cylinder.obj");

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
}

void Cylinder::Update()
{

}

void Cylinder::Draw()
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
