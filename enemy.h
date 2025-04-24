#pragma once
#include "gameObject.h"
#include "component.h"

struct Node
{
	int Id;
	float gCost;	//開始ノードからのコスト
	float hCost;	//ヒューリスティック(ゴールまでの推定コスト)
	int parent;		//経路復元用

	float fCost() const { return gCost + hCost; }
	bool operator > (const Node& other) const { return fCost() > other.fCost(); }
};

struct Node2
{
	int x, y;
	int Id;
	float gCost;	//開始ノードからのコスト
	float hCost;	//ヒューリスティック(ゴールまでの推定コスト)
	int parent;		//経路復元用
	Node2* parentNode;

	float fCost() const { return gCost + hCost; }
	bool operator > (const Node& other) const { return fCost() > other.fCost(); }
};

class Enemy : public GameObject 
{
private:
	int m_Life = 0;
	int m_CreateBulletFrame = 0;
	float m_GroundHeight = 0.0f;
	float m_Speed = 0.0f;

	int m_AttackFrame = 0;
	bool m_IsAttack = false;

	int m_DyingFrame = 0;		//死ぬときのアニメーションのフレーム
	bool m_IsDethAnim = false;
	int m_DieFrame = 0;			//死んでからのフレーム
	bool m_IsDie = false;

	struct WayPoint
	{
		int Id;
		XMFLOAT3 pos;
		std::vector<int> neighbors;
	};



	std::vector<WayPoint> waypoints = {
		{0, {30.0f, 0.0f, 5.0f}, {1}},
		{1, {25.0f, 0.0f, -20.0f}, {0,2}},
		{2, {15.0f, 0.0f, 10.0f}, {1}}
	};


public:

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void EnemyCollision();
	void DeathAnim();

	void SetSpeed(float speed) { m_Speed = speed; }
	float GetSpeed()const { return m_Speed; }

	void MoveAI(XMFLOAT3& pos, const std::vector<WayPoint>& waypoints, const std::vector<int> path, float speed);

	float Heuristic(const XMFLOAT3& a, const XMFLOAT3& b) {
		return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
	}


	//A*探索の関数
	std::vector<int>AStar(const std::vector<WayPoint>& waypoints, int startID, int goalID);
	std::vector<Node2*> AStar(Node2* start, Node2* goal, const std::vector<std::vector<bool>>& grid);

	void DrawImGui()override;

};