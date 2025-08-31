#pragma once
#include "gameObject.h"
#include "component.h"

class Rifle : public GameObject
{
private:

	int m_frame = 0;
	bool m_isHit = false;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	XMFLOAT3 GetMatrixTranslation(const XMMATRIX& mat)
	{
		XMFLOAT3 result;
		XMStoreFloat3(&result, mat.r[3]); 
		return result;
	}

	void DrawImGui()override;
};