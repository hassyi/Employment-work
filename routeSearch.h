#pragma once
#include "main.h"
#include <queue>

struct WayPoint
{
	int Id;
	XMFLOAT3 pos;
	std::vector<int> neighbors;
};

struct Node
{
	int Id;			
	float gCost;	//開始ノードからのコスト
	float hCost;	//ヒューリスティック(ゴールまでの推定コスト)
	float fCost() { return gCost + hCost; }
	int parent;		//経路復元用
};

//ヒューリスティック関数
float Heuristic(const XMFLOAT3& a, const XMFLOAT3& b) {
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}

//A*探索の関数
std::vector<int>AStar(const std::vector<WayPoint>& waypoints, int startID, int goalID)
{
	std::priority_queue < Node, std::vector<Node>, std::greater<>> openList;
	std::unordered_map<int, Node> allNodes;

	openList.push({ startID, 0, Heuristic(waypoints[startID].pos, waypoints[goalID].pos), -1 });

	while (!openList.empty())
	{
		Node current = openList.top();
		openList.pop();

		if (current.Id == goalID)
		{
			std::vector<int> path;
			for (int at = goalID; at != -1; at = allNodes[at].parent)
			{
				path.push_back(at);
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		for (int neighborID : waypoints[current.Id].neighbors)
		{
			float newG = current.gCost + Heuristic(waypoints[current.Id].pos, waypoints[neighborID].pos);
			if (!allNodes.count(neighborID) || newG < allNodes[neighborID].gCost)
			{
				allNodes[neighborID] = { neighborID, newG, Heuristic(waypoints[neighborID].pos, waypoints[goalID].pos), current.Id };
				openList.push(allNodes[neighborID]);
			}
		}
	}
	return{};

}