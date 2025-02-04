#pragma once

#include "component.h"

class GameObject;

class GameObjectComponet : public Component
{
protected:

	struct Angle
	{
		XMFLOAT3 AngleX{};
		XMFLOAT3 AngleY{};
		XMFLOAT3 AngleZ{};
	};

	GameObject* m_GameObject = nullptr;

public:
	GameObjectComponet() = default;
	GameObjectComponet(GameObject* gameobj) : m_GameObject(gameobj) {}

	virtual ~GameObjectComponet() {};

	virtual void Init() override{}
	virtual void Uninit()override{}
	virtual void Update()override{}
	virtual void Draw()override{}

	void AcceptGameObject(GameObject* gameobj) { m_GameObject = gameobj; }
	GameObject* GetGameObject() { return m_GameObject; }


	Angle GetAddAngle(XMFLOAT3 rot, XMFLOAT3 vec)
	{
		float x = vec.x;
		float y = vec.y;
		float z = vec.z;

		//Pich
		float cosP = cosf(rot.x);
		float sinP = sinf(rot.x);

		//Yaw
		float cosY = cosf(rot.y);
		float sinY = sinf(rot.y);

		//Roll
		float cosR = cosf(rot.z);
		float sinR = sinf(rot.z);

		Angle angle;

		angle.AngleX = { x * (cosR * cosY + sinR * sinP * sinY),y * (sinR * cosP),z * (sinR * sinP * cosY - cosR * sinY) };
		angle.AngleY = { x * (cosR * sinP * sinY - sinR * cosY),y * (cosR * cosP),z * (sinR * sinY + cosR * sinP * cosY) };
		angle.AngleZ = { x * (cosP * sinY),y * (-sinP),z * (cosP * cosY) };

		return angle;
	}

};