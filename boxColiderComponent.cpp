#include "boxColiderComponent.h"
#include "transform.h"
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



	return std::tuple<bool, GameObject*, std::list<GameObject*>>();
}

