#include "transform3DComponent.h"
#include "main.h"

void Transform3DComponent::Init()
{
	m_Model = new ModelRenderer;

	if (m_ModelData != "none")	m_Model->Load(m_ModelData);
		

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void Transform3DComponent::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	delete m_Model;
}

void Transform3DComponent::Update()
{

}

void Transform3DComponent::Draw()
{
	if (m_ModelData == "none") return;
	if (m_Model == nullptr) return;

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
	world = m_LocalMatrix * world;
	Renderer::SetWorldMatrix(world);

	//// ラスタライザステート設定
	//D3D11_RASTERIZER_DESC rasterizerDesc{};
	//rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//rasterizerDesc.CullMode = D3D11_CULL_BACK;
	//rasterizerDesc.DepthClipEnable = TRUE;
	//rasterizerDesc.MultisampleEnable = FALSE;
	//rasterizerDesc.DepthBias = 1;
	//rasterizerDesc.SlopeScaledDepthBias = 1.0f;

	//ID3D11RasterizerState* rs;
	//Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs);

	//Renderer::GetDeviceContext()->RSSetState(rs);

	m_Model->Draw();

}
