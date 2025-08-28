#include "transform3DComponent.h"
#include "main.h"

void Transform3DComponent::Init()
{
	m_Model = new ModelRenderer;

	if (m_ModelData != "none")	m_Model->Load(m_ModelData);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		m_VSName);

	Renderer::CreatePixelShader(&m_PixelShader,
		m_PSName);
}

void Transform3DComponent::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	m_Model->Uninit();
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
	trans = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);

	//XMVECTOR v = XMQuaternionRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	//rot = XMMatrixRotationQuaternion(v);

	world = scale * rot * trans;
	world = m_LocalMatrix * world;
	Renderer::SetWorldMatrix(world);

	m_Model->Draw();

}
