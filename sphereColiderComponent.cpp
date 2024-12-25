#include "sphereColiderComponent.h"
#include "transform3DComponent.h"
#include "game.h"
#include "scene.h"

void SphereColiderComponent::Init()
{
	m_Model = new ModelRenderer;
	m_Model->Load("asset\\model\\sphere_smooth.obj");

	m_Pos = GetGameObject()->GetComponent<Transform3DComponent>()->GetPos();
	m_Scale = GetGameObject()->GetComponent<Transform3DComponent>()->GetScale();
	m_Rot = GetGameObject()->GetComponent<Transform3DComponent>()->GetRot();

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\wireFrameVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\wireFramePS.cso");
}

void SphereColiderComponent::Uninit()
{
}

void SphereColiderComponent::Update()
{
	m_Pos = GetGameObject()->GetComponent<Transform3DComponent>()->GetPos();
	m_Scale = GetGameObject()->GetComponent<Transform3DComponent>()->GetScale();
	m_Rot = GetGameObject()->GetComponent<Transform3DComponent>()->GetRot();
}

void SphereColiderComponent::Draw()
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

	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 1;
	rasterDesc.SlopeScaledDepthBias = 1.0f;

	ID3D11RasterizerState* wireframeState;
	Renderer::GetDevice()->CreateRasterizerState(&rasterDesc, &wireframeState);

	Renderer::GetDeviceContext()->RSSetState(wireframeState);

	m_Model->Draw();

	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;

	Renderer::GetDevice()->CreateRasterizerState(&rasterDesc, &wireframeState);
	Renderer::GetDeviceContext()->RSSetState(wireframeState);
}

std::tuple<bool, GameObject*, std::list<GameObject*>> SphereColiderComponent::GetCollision()
{
	int objSize = 0;
	std::list<GameObject*> objectList;
	std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject;

	XMFLOAT3 position;
	XMFLOAT3 scale;

	for (auto object : Scene::GetInstance()->GetScene<Game>()->GetGameObjectList())
	{
		if (object->GetComponent<Colider>() == nullptr) { continue; }
		if (GetGameObject() == object) { continue; }

		position = object->GetComponent<Colider>()->GetPos();
		scale = object->GetComponent<Colider>()->GetScale();

		XMFLOAT3 direction;
		direction.x = position.x - m_Pos.x;
		direction.y = position.y - m_Pos.y;
		direction.z = position.z - m_Pos.z;

		float length = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

		if (length < scale.x)
		{
			objectList.push_back(object);
			objSize = objectList.size();
		}

	}
	if (objSize != 0)
	{
		auto itr = objectList.begin();
		GameObject* gameObject = (*itr);

		OnCollisionObject = std::make_tuple(true, gameObject, objectList);
		return OnCollisionObject;
	}
	else if (objSize == 0)
	{
		OnCollisionObject = std::make_tuple(false, nullptr, objectList);
		return OnCollisionObject;
	}
}

