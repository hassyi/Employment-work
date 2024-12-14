#pragma once
#include "gameObject.h"
#include "component.h"

class BonFire : public GameObject {
private:
	Component* m_Component{};

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

};