#pragma once
#include "main.h"
#include "renderer.h"
#include <list>
#include "colider.h"
#include "sceneState.h"
#include "imguiManager.h"

typedef enum
{
	NONE = 0,
	BOX,
	CYLINDER,
	BUILDING,
	BULLET,
	BULLET_ENEMY,
	PLAYER,
	PREDATION,
	ENEMY,
	SLASH,

	MAX

}OBJ_TYPE;

class Component;

class GameObject {
private:
	XMFLOAT3 m_Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_oldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Vel = { 0.0f,0.0f,0.0f };

protected:
	SceneState* m_SceneState = nullptr;
	std::list<Component*> m_ComponentList;
	bool m_Destroy = false;

	OBJ_TYPE m_ObjType = OBJ_TYPE::NONE;

	float m_Life = 0;

	float m_Gravity = 0.0f;
	bool m_IsGravity = true;
	float m_GroundHeight = 0.0f;

	XMFLOAT3 m_Add = { 0.0f,0.0f,0.0f };

public:
	GameObject(){}
	~GameObject() {};
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
	void SetVel(XMFLOAT3 vel) { m_Vel = vel; }
	void SetVelX(float velx) { m_Vel.x = velx; }
	void SetVelY(float vely) { m_Vel.y = vely; }
	void SetVelZ(float velz) { m_Vel.z = velz; }
	void SetDestroy() { m_Destroy = true; }
	void SetScene(SceneState* scene) { m_SceneState = scene; }
	void SetLife(float life) { m_Life = life; }
	void SetIsGravity(bool isGravity) { m_IsGravity = isGravity; }
	void SetGravityScale(float gravity) { m_Gravity = gravity; }
	void SetAdd(XMFLOAT3 add) { m_Add = add; }
	void SetGuroundHeiht(float height) { m_GroundHeight = height; }

	XMFLOAT3 GetPos() const{	return m_Pos;	}
	XMFLOAT3 GetOldPos() const { return m_oldPos; }
	XMFLOAT3 GetScale() const{	return m_Scale;	}
	XMFLOAT3 GetRot() const{	return m_Rot;	}
	XMFLOAT3 GetVel() const{ return m_Vel; }
	float GetLife()const { return m_Life; }
	SceneState* GetScene() { return m_SceneState; }
	std::list<Component*> GetComponentList() { return m_ComponentList; }
	OBJ_TYPE GetObjectType() { return m_ObjType; }

	//消す・破壊
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

	Colider* GetColider()
	{
		Colider* colider = GetComponent<Colider>();
		return colider;
	}

	template <class T>
	T* AddComponent()
	{
		T* component = new T;
		component->AcceptGameObject(this);
		m_ComponentList.push_back(component);
		component->Init();

		return component;
	}

	template <class T>
	T* GetComponent()
	{
		for (auto component : m_ComponentList)
		{
			T* ret = dynamic_cast<T*>(component);
			if (ret != nullptr) {
				return ret;
			}
		}
		return nullptr;
	}

	XMFLOAT3 GetForward()
	{
		//オイラー格から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[2]);
		return forward;
	}
	XMFLOAT3 GetRight()
	{
		//オイラー格から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);

		XMFLOAT3 right;
		XMStoreFloat3(&right, rotationMatrix.r[0]);
		return right;
	}
	XMFLOAT3 GetTop()
	{
		//オイラー格から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);

		XMFLOAT3 top;
		XMStoreFloat3(&top, rotationMatrix.r[1]);
		return top;
	}


	virtual void DrawImGui() {};
};