#pragma once
#include "main.h"
#include "gameObject.h"

class MoveObject
{
private:
public:
	XMFLOAT3 m_vel;
	virtual XMFLOAT3 Move() = 0;

	void SetVel(XMFLOAT3 vel) { m_vel = vel; }
	void SetVelX(float velx) { m_vel.x = velx; }
	void SetVelY(float vely) { m_vel.y = vely; }
	void SetVelZ(float velz) { m_vel.z = velz; }

	XMFLOAT3 GetVel() { return m_vel; }
};


class PlayerMove : public MoveObject
{
private:
	GameObject* pGameObject;

	const float PLAYER_MOVE = 2.0f;

public:
	XMFLOAT3 Move()override;
	void SetCamera(GameObject* cam) { pGameObject = cam; }

};