#include "cylinderColiderComponent.h"
#include "gameObject.h"
#include "transform3DComponent.h"
#include "game.h"
#include "scene.h"

void CylinderColiderComponent::Init()
{
	m_Pos = GetGameObject()->GetComponent<Transform3DComponent>()->GetPos();
	m_Scale = GetGameObject()->GetComponent<Transform3DComponent>()->GetScale();
	m_Rot = GetGameObject()->GetComponent<Transform3DComponent>()->GetRot();

	m_ColiderType = CYLINDER_COLIDER;

}

void CylinderColiderComponent::Uninit()
{

}

void CylinderColiderComponent::Update()
{
	m_Pos = GetGameObject()->GetComponent<Transform3DComponent>()->GetPos();
	m_Scale = GetGameObject()->GetComponent<Transform3DComponent>()->GetScale();
	m_Rot = GetGameObject()->GetComponent<Transform3DComponent>()->GetRot();
}

void CylinderColiderComponent::Draw()
{
	if (!m_isDraw) return;

}


std::tuple<bool, GameObject*, std::list<GameObject*>> CylinderColiderComponent::GetCollision()
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

		float length = sqrtf(direction.x * direction.x + direction.z * direction.z);

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

void CylinderColiderComponent::MoveCollision()
{
}

