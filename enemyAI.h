#pragma once
#include "gameObjectComponent.h"
#include "imguiManager.h"

class Enemy;

class EnemyAI : public GameObjectComponet
{
private:
	class BehaviorNode* m_BehaviorRoot;

public:
	void SetNode(Enemy* enemy);
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void DrawImGui();
};