#include "playerShadow.h"
#include "animationModel.h"
#include "input.h"
#include "camera.h"
#include "scene.h"
#include "manager.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "game.h"
#include "buffParticle.h"

void PlayerShadow::Init()
{
	//m_Component = new AnimationModel(this);
	//((AnimationModel*)m_Component)->Load("asset\\model\\Vampire A Lusth.fbx");
	//((AnimationModel*)m_Component)->LoadAnimation("asset\\model\\Idle.fbx", "Idle");
	//((AnimationModel*)m_Component)->LoadAnimation("asset\\model\\Idle_Title.fbx", "Idle_Title");
	//((AnimationModel*)m_Component)->LoadAnimation("asset\\model\\Running.fbx", "Run");


	//AddComponent<Transform3DComponent>();
	//GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(0.01f, 0.01f, 0.01f));

	//for (auto component : m_ComponentList)
	//{
	//	component->Init();
	//}

	//m_AnimationName1 = "Idle";
	//m_AnimationName2 = "Idle";

	//Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
	//	"shader\\DepthShadowMappingVS.cso");

	//Renderer::CreatePixelShader(&m_PixelShader,
	//	"shader\\DepthShadowMappingPS.cso");

	m_Component = new ModelRenderer(this);
	((ModelRenderer*)m_Component)->Load("asset\\model\\sphere_smooth.obj");

	SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void PlayerShadow::Uninit()
{
	//m_Component->Uninit();
	//delete m_Component;

	//for (auto component : m_ComponentList)
	//{
	//	component->Uninit();
	//	delete component;
	//}

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void PlayerShadow::Update()
{
	//for (auto component : m_ComponentList)
	//{
	//	component->Update();
	//}
}

void PlayerShadow::Draw()
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

	//for (auto component : m_ComponentList)
	//{
	//	component->Draw();
	//}


	//((AnimationModel*)m_Component)->UpdateAnimation(m_AnimationName1.c_str(), m_AnimationFrame,
	//										m_AnimationName2.c_str(), m_AnimationFrame,
	//										m_AnimetionBlendRatio);
	//m_AnimationFrame++;
	//

	////入力レイアウト設定
	//Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	////シェーダー設定
	//Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	//Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	////ワールドマトリクス設定
	//XMMATRIX world, scale, rot, trans;
	//scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	//rot = XMMatrixRotationRollPitchYaw(GetRot().x, GetRot().y, GetRot().z);
	//trans = XMMatrixTranslation(GetPos().x, GetPos().y, GetPos().z);
	//world = scale * rot * trans;
	//Renderer::SetWorldMatrix(world);

	//ID3D11ShaderResourceView* shadowTexture = Renderer::GetShadowDepthTexture();
	//Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadowTexture);

	//m_Component->Draw();
}
