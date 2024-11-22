#pragma once
#include "gameObjectComponent.h"
#include <tuple>

class Colider : public GameObjectComponet
{
protected:
	XMFLOAT3 m_Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_oldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rot = { 0.0f,0.0f,0.0f };

public:
	Colider(){}
	virtual ~Colider() {};

	virtual void Init()override {};
	virtual void Uninit()override {};
	virtual void Update()override {};
	virtual void Draw()override {};

	void SetTransform(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rot)
	{
		m_Pos = pos;
		m_Scale = scale;
		m_Rot = rot;
	}

	void SetPos(XMFLOAT3 pos) { m_Pos = pos; }
	void SetPosX(float posx) { m_Pos.x = posx; }
	void SetPosY(float posy) { m_Pos.y = posy; }
	void SetPosZ(float posz) { m_Pos.z = posz; }
	void SetOldPos(XMFLOAT3 oldpos) { m_oldPos = oldpos; }
	void SetOldPosX(float oldposx) { m_oldPos.x = oldposx; }
	void SetOldPosY(float oldposy) { m_oldPos.y = oldposy; }
	void SetOldPosZ(float oldposz) { m_oldPos.z = oldposz; }
	void SetScale(XMFLOAT3 scale) { m_Scale = scale; }
	void SetScaleX(float scalex) { m_Scale.x = scalex; }
	void SetScaleY(float scaley) { m_Scale.y = scaley; }
	void SetScaleZ(float scalez) { m_Scale.z = scalez; }
	void SetRot(XMFLOAT3 rot) { m_Rot = rot; }
	void SetRotX(float rotx) { m_Rot.x = rotx; }
	void SetRotY(float roty) { m_Rot.y = roty; }
	void SetRotZ(float rotz) { m_Rot.z = rotz; }

	XMFLOAT3 GetPos() { return m_Pos; }
	XMFLOAT3 GetScale() { return m_Scale; }
	XMFLOAT3 GetRot() { return m_Rot; }

	//virtual std::tuple<bool, GameObject*, std::list<GameObject*>> GetCollision() = 0;
};