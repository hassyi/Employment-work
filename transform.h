#pragma once
#include "gameObjectComponent.h"
#include "renderer.h"
#include "modelRenderer.h"

class Transform : public GameObjectComponet
{
protected:
	XMFLOAT3 m_Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_oldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Vel = { 0.0f,0.0f,0.0f };

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	float m_Angle = 0.0f;
	float m_Length = 0.0f;
	float m_Fai = 0.0f;

public:
	Transform() = default;
	Transform(GameObject* gameobj) : GameObjectComponet(gameobj){}

	virtual void Init()override {};
	virtual void Uninit()override {};
	virtual void Update()override {};
	virtual void Draw()override {};

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
	void SetVel(XMFLOAT3 vel) { m_Vel = vel; }
	void SetVelX(float velx) { m_Vel.x = velx; }
	void SetVelY(float vely) { m_Vel.y = vely; }
	void SetVelZ(float velz) { m_Vel.z = velz; }

	XMFLOAT3 GetPos() const{ return m_Pos; }
	XMFLOAT3 GetScale() const { return m_Scale; }
	XMFLOAT3 GetRot() const { return m_Rot; }
	XMFLOAT3 GetVel() const { return m_Vel; }

};