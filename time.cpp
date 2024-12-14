#include "time.h"
#include "textureComponent.h"

void Time::Init()
{
	AddComponent<TextureComponent>()->SetTransTexNum(0.0f, 0.0f, 200.0f, 200.0f, L"asset\\texture\\number.png", 1);
	for (auto component : m_ComponentList)
	{
		component->Init();
	}
	GetComponent<TextureComponent>()->SetDisitDisplay(true);
}

void Time::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();

		delete component;
	}
}

void Time::Update()
{
	m_timeCount++;
	if (m_timeCount > 60) {
		m_Count++;
		m_timeCount = 0;
	}
	//if (m_Count > 9) {
	//	m_Count = 0;
	//}
}

void Time::Draw()
{
	int num;
	num = m_Count;

	for (auto component : m_ComponentList)
	{
		for (int i = 0; i < SCORE_DISIT; i++)
		{
			//テクスチャ座標算出
			int score = (num % 10);
			num /= 10;

			float x = (score % 5) * (1.0f / 5);
			float y = (score / 5) * (1.0f / 5);

			GetComponent<TextureComponent>()->SetPosSize(0.0f, 0.0f, 50.0f, 50.0f);
			GetComponent<TextureComponent>()->SetDisit(50.0f * (SCORE_DISIT - 1 - i));
			GetComponent<TextureComponent>()->SetAnimation(x, y, 0.2f, 0.2f);
			component->Draw();
		}
	}
}

