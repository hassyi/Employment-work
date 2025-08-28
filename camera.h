#pragma once
#include "gameObject.h"

class Player;

class Camera : public GameObject
{
	XMFLOAT3 m_StartPos = { 0.0f, 5.0f, -10.0f };
	XMFLOAT3 m_Target = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_AddTarget = { 0.0f,0.0f,0.0 };
	XMFLOAT3 m_AddValue = {1.5f, 0.8f, 1.5f};
	XMFLOAT4X4 m_ViewMatrix{};

	float m_Len = 0.0f;
	const float VALUE_ROTATE_CAMERA = 3.14f * 0.01f;
	const int SENS_MIDDLE = 450;
	int m_CameraCount = 0;

	XMFLOAT2 m_MousePos = { 0.0f,0.0f };
	XMFLOAT2 m_OldmousePos = { 0.0f,0.0f };
	XMFLOAT2 m_Sensitivity = { 0.02f, 0.01f };

	POINT m_MousePoint = {};

	LONG m_ScreenWidthCamera = (LONG)GetWindowSize().x;
	LONG m_ScreenHeightCamera = (LONG)GetWindowSize().y;
	XMFLOAT2 m_WindowSize = GetWindowSize();

	POINT m_ClientCenter;

	int m_CenterCamX = SCREEN_WIDTH / 2;
	int m_CenterCamY = SCREEN_HEIGHT / 2;

	int m_Mouse = 1;		//カメラ操作切り替え
	bool m_IsMouse = true;

	float m_R = 0.0f;
	float m_S = 0.0f;
	float m_F = 0.0f;
	float m_Radian = 0.0f;

	float m_FrameCoutnt[4] = { 0.0f,0.0f,0.0f,0.0f };

	XMFLOAT3 m_AimOffset = {};
	XMFLOAT3 m_AimLookAt = {};

	bool m_IsAim = false;
	bool m_SetLenDoOnce = false;

public:
	void Init() override;
	void Uninit() override;
	void Update()override;
	void Draw()override;

	void CameraControl();
	void SetMouseCamera(XMFLOAT3 pos);
	void SetKeyCamera();
	void AimCameraControl();
	bool CheckView(XMFLOAT3 pos, float size);

	void SetTarget(XMFLOAT3 target) { m_Target = target; }
	void SetTargetX(float targetx) { m_Target.x = targetx; }
	void SetTargetY(float targety) { m_Target.y = targety; }
	void SetTargetZ(float targetz) { m_Target.z = targetz; }

	XMFLOAT3 GetTarget() const { return m_Target; }
	XMMATRIX GetViewMatrix()
	{
		return XMLoadFloat4x4(&m_ViewMatrix);
	}

	void DrawImGui() override;

};