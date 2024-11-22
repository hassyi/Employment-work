#pragma once
#include "transform.h"


class Transform3DComponent : public Transform
{
private:
	ModelRenderer* m_Model = nullptr;
	const char* m_ModelData = "none";

	float m_Rotation = 0.0f;


public:
	Transform3DComponent() = default;
	Transform3DComponent(GameObject* gameobj) : Transform(gameobj){}
	Transform3DComponent(GameObject* gameobj, const char* data) : Transform(gameobj), m_ModelData(data){}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void AddModelData(const char* data)
	{
		m_ModelData = data;
		m_Model->Load(m_ModelData);
	}

	void SetTransform(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rot)
	{
		m_Pos = pos;
		m_Scale = scale;
		m_Rot = rot;
	}

	void SetOldPos(XMFLOAT3 oldpos) { m_oldPos = oldpos; }

	XMFLOAT3 GetOldPos() const { return m_oldPos; }

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


};