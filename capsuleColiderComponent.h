#pragma once
#include "colider.h"

class GameObject;
class BoxColiderComponent;

class CapsuleColiderComponent : public Colider
{
private:

	Capsule m_Capsule =
	{
		XMFLOAT3(0.0f,0.0f,0.0f),
		XMFLOAT3(0.0f,0.0f,0.0f),
		0.0f
	};

	//始点
	XMFLOAT3 m_StartPos = {};
	//終点
	XMFLOAT3 m_EndPos = {};
	//中心から上と下の長さ
	float m_CenterLength = 0.0f;


public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	float SegmentDistance(const XMFLOAT3 vec1start, const XMFLOAT3 vec1end, const XMFLOAT3 vec2start, const XMFLOAT3 vec2end, float& t1, float& t2);
	bool CapsuleCollision(const CapsuleColiderComponent* colider1, const CapsuleColiderComponent* colider2);
	bool BoxCollision(Capsule& capsule, GameObject* object);
	bool SphereCollision(Capsule& capsule, GameObject* object);

	std::tuple<bool, GameObject*, std::list<GameObject*>> GetCollision()override;
	void MoveCollision()override;
	
	Capsule GetCapsule()const { return m_Capsule; }
	void SetSegmentLength(float length) { m_CenterLength = length; }

	float RaySegmentDistance(const XMFLOAT3& p1, const XMFLOAT3& q1, const XMFLOAT3& p2, const XMFLOAT3& q2, float& s, float& t);
	bool RayCollision(const XMFLOAT3 rayOrigin, const XMFLOAT3 rayDir, const XMFLOAT3 capsuleStart, const XMFLOAT3 capsuleEnd, float capsuleRadius);

};
