#pragma once
#include "gameObject.h"
#include "UI.h"

class AimIcon : public UI
{
private:
	XMFLOAT2 m_Pos{};
	XMFLOAT2 m_Size{};

	XMFLOAT2 m_CenterPosition = { 0.0f,0.0f };
	float m_Radius = 80.0f;

	XMMATRIX m_ViewMatrix = {};
	XMMATRIX m_ProjectionMatrix = {};


	bool m_IsDraw = false;

public:
	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	bool ObjectInCircle(XMFLOAT3 worldPos, int screenWidth, int screenHeight, float& screenPosX, float& screenPosY);

	void SetViewMatrix(XMMATRIX view) { m_ViewMatrix = view; }
	void SetProjectionMatrix(XMMATRIX projection) { m_ProjectionMatrix = projection; }

	void SetIsDraw(bool isDraw) { m_IsDraw = isDraw; }
};