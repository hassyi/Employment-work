#include "lifeGauge.h"
#include "textureComponent.h"
#include "texture2D.h"

void LifeGauge::Init()
{
	AddComponent<TextureComponent>()->SetTransTexNum(10.0f, 10.0f, 250.0f, 20.0f, L"asset\\texture\\statusGauge.png", 1);
	GetComponent<TextureComponent>()->SetDisitDisplay(false);
	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void LifeGauge::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();

		delete component;
	}
}

void LifeGauge::Update()
{
	
}

void LifeGauge::Draw()
{
	for (auto component : m_ComponentList)
	{
		GetComponent<TextureComponent>()->SetPosSize(10.0f, 10.0f, 250.0f, 20.0f);
		GetComponent<TextureComponent>()->SetAnimation(0.0f, 0.0f, 1.0f, 0.33f);
		component->Draw();
		
	}
}

