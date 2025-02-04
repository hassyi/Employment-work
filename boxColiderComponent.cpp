#include "boxColiderComponent.h"
#include "gameObject.h"
#include "transform3DComponent.h"
#include "game.h"
#include "scene.h"

void BoxColiderComponent::Init()
{
	m_Model = new ModelRenderer;
	m_Model->Load("asset\\model\\box.obj");

	m_Pos = GetGameObject()->GetComponent<Transform>()->GetPos();
	m_Scale = GetGameObject()->GetComponent<Transform>()->GetScale();
	m_Rot = GetGameObject()->GetComponent<Transform>()->GetRot();

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\wireFrameVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\wireFramePS.cso");

	m_ColiderType = BOX_COLIDER;
}

void BoxColiderComponent::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	delete m_Model;
}

void BoxColiderComponent::Update()
{
	m_Pos = GetGameObject()->GetComponent<Transform>()->GetPos();
	m_Rot = GetGameObject()->GetComponent<Transform>()->GetRot();

	m_Pos = Add(m_Pos, m_AddPos);

	MoveCollision();
}

void BoxColiderComponent::Draw()
{
	//DrawImGui();

	if (!Scene::GetInstance()->GetScene<Game>()->GetIsDrawColider()) return;

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	if (GetGameObject()->GetObjectType() == OBJ_TYPE::BOX)	{
		trans = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	}
	else {
		trans = XMMatrixTranslation(m_Pos.x, m_Pos.y + m_Scale.y, m_Pos.z);
	}
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

std::tuple<bool, GameObject*, std::list<GameObject*>> BoxColiderComponent::GetCollision()
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

		if (m_Pos.z - (m_Scale.z) <= position.z + (scale.z) &&
			m_Pos.z + (m_Scale.z) >= position.z - (scale.z) &&
			m_Pos.x - (m_Scale.x) <= position.x + (scale.x) &&
			m_Pos.x + (m_Scale.x) >= position.x - (scale.x) &&
			m_Pos.y - (m_Scale.y * 2.0f) <= position.y + (scale.y * 2.0f) &&	//上面
			m_Pos.y + (m_Scale.y) >= position.y - (scale.y)						//下面
			)
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

std::tuple<bool, GameObject*, std::list<GameObject*>> BoxColiderComponent::GetCollisionOBB()
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
		direction.x = m_Pos.x - position.x;
		direction.y = m_Pos.y - position.y;
		direction.z = m_Pos.z - position.z;

		//X分離軸
		XMFLOAT3 axisX = object->GetForward();
		float dotX = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;

		//Z分離軸
		XMFLOAT3 axizZ = object->GetRight();
		float dotZ = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;

		//Y分離軸
		XMFLOAT3 axizY = object->GetTop();
		float dotY = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;

		if (-scale.x < dotX && dotX < scale.x &&
			-scale.z < dotZ && dotZ < scale.z &&
			-scale.y < dotY && dotY < scale.y * 2.0f)
		{
			objectList.push_back(object);
			objSize = objectList.size();
		}

		if (objSize != 0)
		{
			auto itr = objectList.begin();
			GameObject* gameObject = (*itr);

			OnCollisionObject = std::make_tuple(true, gameObject, objectList);
			return OnCollisionObject;
		}
		else
		{
			OnCollisionObject = std::make_tuple(false, nullptr, objectList);
			return OnCollisionObject;
		}

	}
}

void BoxColiderComponent::MoveCollision()
{
	XMFLOAT3 vector = XMFLOAT3(1.0f, 1.0f, 1.0f);
	Angle angle;
	angle = GetAddAngle(m_Rot, vector);

	m_Box = { m_Pos,
			 XMFLOAT3(m_Scale.x, m_Scale.y, m_Scale.z),
			 angle.AngleX, angle.AngleY, angle.AngleZ };

}

void BoxColiderComponent::DrawImGui()
{
	

	{
		ImGui::Begin("BoxColider");

		ImGui::Text("ColiderPos : x = %.1f, y = %.1f, z = %.1f", m_Pos.x, m_Pos.y, m_Pos.z);
		ImGui::Text("ColiderScale : x = %.1f, y = %.1f, z = %.1f", m_Scale.x, m_Scale.y, m_Scale.z);
		ImGui::Text("BoxAxiz0 : x = %.1f, y = %.1f, z = %.1f", m_Box.axiz[0].x, m_Box.axiz[0].y, m_Box.axiz[0].z);
		ImGui::Text("BoxAxiz0 : x = %.1f, y = %.1f, z = %.1f", m_Box.axiz[1].x, m_Box.axiz[1].y, m_Box.axiz[1].z);
		ImGui::Text("BoxAxiz0 : x = %.1f, y = %.1f, z = %.1f", m_Box.axiz[2].x, m_Box.axiz[2].y, m_Box.axiz[2].z);

		ImGui::End();
	}
}

