#include "result.h"
#include "input.h"
#include "manager.h"
#include "title.h"
#include "texture2D.h"

void Result::Init()
{
	AddUITexture<Texture2D>()->SetTransTexNum(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, L"asset\\texture\\clearresult.png", 1);

	m_Satate = SCENE_STATE::SCENE_CLEAR_RESULT;

	for (auto texture : m_Texture)
	{
		texture->Init();
	}
	m_BGM = new Audio();
	m_BGM->Load("asset\\audio\\clearBGM.wav");
	m_BGM->SetSound(0.5f);
	m_BGM->Play(true);

}

void Result::Uninit()
{
	m_BGM->Uninit();
	delete m_BGM;

	for (auto texture : m_Texture)
	{
		texture->Uninit();
		delete texture;
	}
}

void Result::Update()
{
	for (auto texture : m_Texture)
	{
		texture->Update();
	}

	if (Input::GetKeyTrigger(VK_RETURN)) {
		Scene::GetInstance()->ChangeScene(new Title);
	}
}

void Result::Draw()
{
	Renderer::Begin();
	for (auto texture : m_Texture)
	{
		texture->Draw();
	}
	Renderer::End();
}
