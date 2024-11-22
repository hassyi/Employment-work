#pragma once

#include "component.h"

class GameObject;

class GameObjectComponet : public Component
{
protected:
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

};