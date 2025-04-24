#include "bulletGauge.h"
#include "textureComponent.h"
#include "texture2D.h"
#include "scene.h"
#include "game.h"
#include "player.h"

void BulletGauge::Init()
{
	m_Pos = XMFLOAT2(10.0f, 40.0f);
	m_Size = XMFLOAT2(400.0f, 20.0f);

	AddComponent<TextureComponent>()->SetTransTexNum(m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, L"asset\\texture\\statusGauge.png", 1);
	GetComponent<TextureComponent>()->SetDisitDisplay(false);
	for (auto component : m_ComponentList)
	{
		component->Init();
	}

	m_UV = XMFLOAT2(1.0f, 0.33f);
	m_BP_Max = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->GetBulletPoint();

}

void BulletGauge::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();

		delete component;
	}
}

void BulletGauge::Update()
{
	float bp = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->GetBulletPoint();;

	float bpCount = m_Size.x / m_BP_Max;
	m_ChangeSize = bpCount * (bp - m_BP_Max);

	float bpCount_U = 1.0f / m_BP_Max;

	m_UV.x = bpCount_U * bpCount;
}

void BulletGauge::Draw()
{
	for (auto component : m_ComponentList)
	{
		GetComponent<TextureComponent>()->SetPosSize(m_Pos.x, m_Pos.y, m_Size.x + m_ChangeSize, m_Size.y);
		GetComponent<TextureComponent>()->SetAnimation(0.0f, 0.33f, m_UV.x, m_UV.y);
		component->Draw();
		
	}
}

