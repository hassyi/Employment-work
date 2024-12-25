#pragma once
#include "gameObjectComponent.h"
#include <tuple>

class ModelRenderer;

class Colider : public GameObjectComponet
{
protected:
	XMFLOAT3 m_Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_OldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rot = { 0.0f,0.0f,0.0f };

	ModelRenderer* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

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
	void SetOldPos(XMFLOAT3 oldpos) { m_OldPos = oldpos; }
	void SetOldPosX(float oldposx) { m_OldPos.x = oldposx; }
	void SetOldPosY(float oldposy) { m_OldPos.y = oldposy; }
	void SetOldPosZ(float oldposz) { m_OldPos.z = oldposz; }
	void SetScale(XMFLOAT3 scale) { m_Scale = scale; }
	void SetScaleX(float scalex) { m_Scale.x = scalex; }
	void SetScaleY(float scaley) { m_Scale.y = scaley; }
	void SetScaleZ(float scalez) { m_Scale.z = scalez; }
	void SetRot(XMFLOAT3 rot) { m_Rot = rot; }
	void SetRotX(float rotx) { m_Rot.x = rotx; }
	void SetRotY(float roty) { m_Rot.y = roty; }
	void SetRotZ(float rotz) { m_Rot.z = rotz; }

	XMFLOAT3 GetPos() const { return m_Pos; }
	XMFLOAT3 GetOldPos() const { return m_OldPos; }
	XMFLOAT3 GetScale()const { return m_Scale; }
	XMFLOAT3 GetRot() const { return m_Rot; }

	virtual std::tuple<bool, GameObject*, std::list<GameObject*>> GetCollision() = 0;
};