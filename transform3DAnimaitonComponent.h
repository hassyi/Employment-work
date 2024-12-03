#pragma once
#include "transform.h"
#include "animationModel.h"

class Transform3DAnimationComponent : public Transform
{
private:

	AnimationModel* m_Model = nullptr;
	const char* m_ModelData = "none";
	const char* m_AnimationData = "none";
	const char* m_AnimationName = "none";

	float m_Rotation = 0.0f;


public:
	Transform3DAnimationComponent() = default;
	Transform3DAnimationComponent(GameObject* gameobj) : Transform(gameobj) {}
	Transform3DAnimationComponent(GameObject* gameobj, const char* data) : Transform(gameobj), m_ModelData(data) {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void AddModelData(const char* data, GameObject* gameobj)
	{
		m_Model = new AnimationModel(gameobj);
		m_ModelData = data;
		m_Model->Load(m_ModelData);
	}

	void AddAnimationData(const char* data, const char* name)
	{
		m_AnimationData = data;
		m_AnimationName = name;
		m_Model->LoadAnimation(m_AnimationData, m_AnimationName);
	}

	void SetTransform(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rot)
	{
		m_Pos = pos;
		m_Scale = scale;
		m_Rot = rot;
	}

	void SetOldPos(XMFLOAT3 oldpos) { m_oldPos = oldpos; }

	XMFLOAT3 GetOldPos() const { return m_oldPos; }

};