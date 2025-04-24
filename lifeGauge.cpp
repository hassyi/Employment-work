#include "lifeGauge.h"
#include "textureComponent.h"
#include "texture2D.h"
#include "scene.h"
#include "game.h"
#include "player.h"

void LifeGauge::Init()
{
	m_Pos = XMFLOAT2(10.0f, 10.0f);
	m_Size = XMFLOAT2(400.0f, 20.0f);

	AddComponent<TextureComponent>()->SetTransTexNum(m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, L"asset\\texture\\statusGauge.png", 1);
	GetComponent<TextureComponent>()->SetDisitDisplay(false);
	for (auto component : m_ComponentList)
	{
		component->Init();
	}
	m_UV = XMFLOAT2(1.0f, 0.33f);
	m_LifeMax = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->GetLife();
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

	float playerLife = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->GetLife();

	float lifeCount = m_Size.x / m_LifeMax;
	m_ChangeSize = lifeCount * (playerLife - m_LifeMax);

	float lifeCunt_U = 1.0f / m_LifeMax;

	m_UV.x = lifeCunt_U * playerLife;

}

void LifeGauge::Draw()
{
	for (auto component : m_ComponentList)
	{
		GetComponent<TextureComponent>()->SetPosSize(m_Pos.x, m_Pos.y, m_Size.x + m_ChangeSize, m_Size.y);
		GetComponent<TextureComponent>()->SetAnimation(0.0f, 0.0f, m_UV.x, m_UV.y);
		component->Draw();
		
	}
}

