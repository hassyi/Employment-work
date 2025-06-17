#pragma once
#include "gameObject.h"

class TitleCamera : public GameObject
{
private:
	XMFLOAT3 m_Target = { 0.0f,0.0f,0.0f };
	XMFLOAT4X4 m_ViewMatrix{};

	float m_len;
	const float VALUE_ROTATE_CAMERA = 3.14f * 0.01f;
	const int SENS_MIDDLE = 450;
	int m_CameraCount = 0;

	float m_R = 0.0f;
	float m_S = 0.0f;
	float m_F = 0.0f;
	float m_Radian = 0.0f;

	float m_FrameCoutnt[4] = { 0.0f,0.0f,0.0f,0.0f };

public:
	void Init() override;
	void Uninit() override;
	void Update()override;
	void Draw()override;

	void SetTarget(XMFLOAT3 target) { m_Target = target; }
	void SetTargetX(float targetx) { m_Target.x = targetx; }
	void SetTargetY(float targety) { m_Target.y = targety; }
	void SetTargetZ(float targetz) { m_Target.z = targetz; }

	XMFLOAT3 GetTarget() const { return m_Target; }
	XMMATRIX GetViewMatrix()
	{
		return XMLoadFloat4x4(&m_ViewMatrix);
	}

};