#pragma once
#include "gameObject.h"
#include "UI.h"

class AimIcon : public UI
{
private:
	bool m_IsDraw = false;

public:
	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetIsDraw(bool isDraw) { m_IsDraw = isDraw; }
};