#include "boxColiderComponent.h"
#include "transform3DComponent.h"
#include "game.h"
#include "scene.h"

void BoxColiderComponent::Init()
{
	m_Pos = GetGameObject()->GetComponent<Transform>()->GetPos();
	m_Scale = GetGameObject()->GetComponent<Transform>()->GetScale();
	m_Rot = GetGameObject()->GetComponent<Transform>()->GetRot();

}

void BoxColiderComponent::Uninit()
{

}

void BoxColiderComponent::Update()
{
	m_Pos = GetGameObject()->GetComponent<Transform>()->GetPos();
	m_Rot = GetGameObject()->GetComponent<Transform>()->GetRot();
}

void BoxColiderComponent::Draw()
{
	//D3D11_RASTERIZER_DESC rasterDesc{};
	//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	//rasterDesc.CullMode = D3D11_CULL_NONE;
	//rasterDesc.DepthClipEnable = TRUE;
	//rasterDesc.DepthBias = 1;
	//rasterDesc.SlopeScaledDepthBias = 1.0f;

	//ID3D11RasterizerState* wireframeState;
	//Renderer::GetDevice()->CreateRasterizerState(&rasterDesc, &wireframeState);

	//Renderer::GetDeviceContext()->RSSetState(wireframeState);

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
			m_Pos.y - (m_Scale.y * 2.0f) <= position.y + (scale.y * 3.0f) &&	//上面
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

