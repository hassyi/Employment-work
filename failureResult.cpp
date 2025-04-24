#include "failureResult.h"
#include "input.h"
#include "manager.h"
#include "title.h"
#include "texture2D.h"

void FailureResult::Init()
{
	AddUITexture<Texture2D>()->SetTransTexNum(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, L"asset\\texture\\failureresult.png", 1);
	m_Satate = SCENE_STATE::SCENE_FAILURE_RESULT;

	for (auto texture : m_Texture)
	{
		texture->Init();
	}
	m_BGM = new Audio();
	m_BGM->Load("asset\\audio\\failureBGM.wav");
	m_BGM->SetSound(0.5f);
	m_BGM->Play(true);

}

void FailureResult::Uninit()
{
	m_BGM->Uninit();
	delete m_BGM;

	for (auto texture : m_Texture)
	{
		texture->Uninit();
		delete texture;
	}
}

void FailureResult::Update()
{
	for (auto texture : m_Texture)
	{
		texture->Update();
	}

	if (Input::GetKeyTrigger(VK_RETURN)) {
		Scene::GetInstance()->ChangeScene(new Title);
	}
}

void FailureResult::Draw()
{
	Renderer::Begin();
	for (auto texture : m_Texture)
	{
		texture->Draw();
	}
	Renderer::End();
}
