#pragma once
#include "gameObject.h"
#include "UI.h"

class GunIcon : public UI
{
private:

public:
	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};