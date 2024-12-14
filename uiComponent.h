#pragma once
#include "component.h"
#include "UI.h"

class UI;

class UIComponent : public Component
{
protected:
	UI* m_UI = nullptr;

public:
	UIComponent() = default;
	UIComponent(UI* ui) : m_UI(ui) {}

	virtual ~UIComponent() {};

	virtual void Init() override {}
	virtual void Uninit()override {}
	virtual void Update()override {}
	virtual void Draw()override {}

	void AcceptUITexture(UI* ui) { m_UI = ui; }
	UI* GetUITexture() { return m_UI; }

};