#pragma once
#include "gameObjectComponent.h"

enum BEHAVIOR_RESULT
{
	BEHAVIOR_RESULT_CONTINUE,
	BEHAVIOR_RESULT_SUCCESS,
	BEHAVIOR_RESULT_FAILURE
};

class BehaviorNode
{
protected:
	std::vector<BehaviorNode*> m_Child;
	class Enemy* m_Enemy;


	const char* m_BehaviorText = "none";
	bool m_IsSuccess = false;

public:
	BehaviorNode() = delete;
	BehaviorNode(Enemy* enemy) { m_Enemy = enemy; }
	~BehaviorNode(){}
	
	void AddChild(BehaviorNode* child) { m_Child.push_back(child); }
	std::vector<BehaviorNode*> GetChildNum() { return m_Child; }
	void Uninit() { m_Child.clear(); }
	virtual BEHAVIOR_RESULT Update() = 0;
	const char* GetBehaviorName() { return m_BehaviorText; }
	bool GetIsSuccess() { return m_IsSuccess; }
};