#include "transform3DAnimaitonComponent.h"

void Transform3DAnimationComponent::Init()
{

	if (m_ModelData == "none") return;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_AddAnimFrame = 1;
}

void Transform3DAnimationComponent::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	delete m_Model;
}

void Transform3DAnimationComponent::Update()
{

}

void Transform3DAnimationComponent::Draw()
{
	if (m_ModelData == "none") return;
	if (m_Model == nullptr) return;

	m_Model->UpdateAnimation(m_AnimationName1.c_str(), m_AnimationFrame1,
		m_AnimationName2.c_str(), m_AnimationFrame2,
		m_AnimetionBlendRatio);
	m_AnimationFrame1 += m_AddAnimFrame;
	m_AnimationFrame2 += m_AddAnimFrame;

	if (m_AnimationName2 != m_AnimationNameState)
	{
		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = m_AnimationNameState;
		m_AnimetionBlendRatio = 0.0f;
	}
	m_AnimetionBlendRatio += 0.1f;
	if (m_AnimetionBlendRatio > 1.0f) {
		m_AnimetionBlendRatio = 1.0f;
	}

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

	m_Model->Draw();
}
