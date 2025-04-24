#pragma once
#include "gameObject.h"
#include <tuple>
#include <list>
#include "box.h"

class Collision
{
private:
	bool m_collisionHeight = false;

public:
	bool CollisionBB(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scl1, XMFLOAT3 scl2);	//箱
	bool CollisionBBHeight(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scl1, XMFLOAT3 scl2);	//箱
	bool CollisionBS(XMFLOAT3 pos1, XMFLOAT3 pos2, float radius);					//球体
	bool CollisionCylinder(XMFLOAT3 pos1, XMFLOAT3 pos2, float radius);				//円柱
	bool CollisionCylinderHeight(XMFLOAT3 pos1, XMFLOAT3 pos2, float height);				//円柱
	bool GetCylinderHeight() { return m_collisionHeight; }

	std::tuple<bool, std::list<Box*>> CollisionBB(XMFLOAT3 pos, XMFLOAT3 scl);
	std::tuple<bool, std::list<Box*>> CollisionOBB(XMFLOAT3 pos, XMFLOAT3 scl);

};