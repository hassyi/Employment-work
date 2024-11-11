#pragma once
#include "main.h"
#include "renderer.h"

class Camera;

typedef enum
{
	NONE = 0,
	PLAYER,
	ENEMY,
	BOX,
	CYLINDER,


}ObjectType;

class GameObject {
private:
	XMFLOAT3 m_Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_oldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Target = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_CameraRot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Vel = { 0.0f,0.0f,0.0f };

	bool m_Destroy = false;



	ObjectType m_Type = NONE;

public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetPos(XMFLOAT3 pos) {	m_Pos = pos;}
	void SetPosX(float posx) {	m_Pos.x = posx;	}
	void SetPosY(float posy) {	m_Pos.y = posy;	}
	void SetPosZ(float posz) {	m_Pos.z = posz;	}
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
	void SetTarget(XMFLOAT3 target) { m_Target = target; }
	void SetTargetX(float targetx) { m_Target.x = targetx; }
	void SetTargetY(float targety) { m_Target.y = targety; }
	void SetTargetZ(float targetz) { m_Target.z = targetz; }
	void SetVel(XMFLOAT3 vel) { m_Vel = vel; }
	void SetVelX(float velx) { m_Vel.x = velx; }					
	void SetVelY(float vely) { m_Vel.y = vely; }
	void SetVelZ(float velz) { m_Vel.z = velz; }
	void SetDestroy() { m_Destroy = true; }
	void SetObjType(ObjectType type) { m_Type = type; }

	XMFLOAT3 GetPos() const{	return m_Pos;	}
	XMFLOAT3 GetOldPos() const { return m_oldPos; }
	XMFLOAT3 GetScale() const{	return m_Scale;	}
	XMFLOAT3 GetRot() const{	return m_Rot;	}
	XMFLOAT3 GetTarget() const{	return m_Target;	}
	XMFLOAT3 GetVel() { return m_Vel; }
	ObjectType GetObjType() { return m_Type; }

	//�����E�j��
	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	XMFLOAT3 GetForward()
	{
		//�I�C���[�i�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[2]);
		return forward;
	}
	XMFLOAT3 GetRight()
	{
		//�I�C���[�i�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);

		XMFLOAT3 right;
		XMStoreFloat3(&right, rotationMatrix.r[0]);
		return right;
	}
	XMFLOAT3 GetTop()
	{
		//�I�C���[�i�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);

		XMFLOAT3 top;
		XMStoreFloat3(&top, rotationMatrix.r[1]);
		return top;
	}

};