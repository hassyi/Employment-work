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
	bool CollisionBB(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scl1, XMFLOAT3 scl2);	//” 
	bool CollisionBBHeight(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scl1, XMFLOAT3 scl2);	//” 
	bool CollisionBS(XMFLOAT3 pos1, XMFLOAT3 pos2, float radius);					//‹…‘Ì
	bool CollisionCylinder(XMFLOAT3 pos1, XMFLOAT3 pos2, float radius);				//‰~’Œ
	bool CollisionCylinderHeight(XMFLOAT3 pos1, XMFLOAT3 pos2, float height);				//‰~’Œ
	bool GetCylinderHeight() { return m_collisionHeight; }

	std::tuple<bool, std::list<Box*>> CollisionBB(XMFLOAT3 pos, XMFLOAT3 scl);

};