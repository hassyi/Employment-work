#pragma once
#include "gameObjectComponent.h"
#include <tuple>
#include "calculation.h"

enum COLIDER_TYPE
{
	BOX_COLIDER,
	CAPSULE_COLIDER,
	CYLINDER_COLIDER,
	SPHERE_COLIDER

};

class ModelRenderer;

class Colider : public GameObjectComponet
{
protected:



	struct Capsule
	{
		XMFLOAT3 startPos, endPos;
		float radius;
	};

	struct Box 
	{
		XMFLOAT3 center;
		XMFLOAT3 size;
		XMFLOAT3 axiz[3];
	};



	XMFLOAT3 m_Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_AddPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_OldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rot = { 0.0f,0.0f,0.0f };

	ModelRenderer* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	COLIDER_TYPE m_ColiderType = BOX_COLIDER;

	bool m_isDraw;

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
	void SetColiderDraw(bool isDraw) { m_isDraw = isDraw; }

	XMFLOAT3 GetPos() const { return m_Pos; }
	XMFLOAT3 GetOldPos() const { return m_OldPos; }
	XMFLOAT3 GetScale()const { return m_Scale; }
	XMFLOAT3 GetRot() const { return m_Rot; }
	bool GetIsDraw()const { return m_isDraw; }

	COLIDER_TYPE GetColiderType() { return m_ColiderType; }

	virtual std::tuple<bool, GameObject*, std::list<GameObject*>> GetCollision() = 0;

	virtual void MoveCollision() = 0;

	XMFLOAT3 LocalSpace(const XMFLOAT3 vec, const Box box)
	{
		XMFLOAT3 relVec = Sub(vec, box.center);
		return XMFLOAT3(Dot(relVec, box.axiz[0]), 
						Dot(relVec, box.axiz[1]), 
						Dot(relVec, box.axiz[2]));
	}

	XMFLOAT3 WorldSpace(const XMFLOAT3& localvec, const Box& box)
	{
		return Add(XMFLOAT3(localvec.x * box.axiz[0].x + localvec.y * box.axiz[1].x + localvec.z * box.axiz[2].x,
							localvec.x * box.axiz[0].y + localvec.y * box.axiz[1].y + localvec.z * box.axiz[2].y,
							localvec.x * box.axiz[0].z + localvec.y * box.axiz[1].z + localvec.z * box.axiz[2].z), box.center);
	}

	XMFLOAT3 clampToBox(const XMFLOAT3 point, const XMFLOAT3 halfSize) {
		return {
			std::max(-halfSize.x, std::min(point.x, halfSize.x)),
			std::max(-halfSize.y, std::min(point.y, halfSize.y)),
			std::max(-halfSize.z, std::min(point.z, halfSize.z))
		};
	}
};