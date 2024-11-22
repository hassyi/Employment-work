#include "result.h"
#include "input.h"
#include "manager.h"
#include "title.h"
#include "texture2D.h"

void Result::Init()
{
	AddUITexture<Texture2D>()->SetTransTexNum(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, L"asset\\texture\\result.png", 1);

	for (auto texture : m_Texture)
	{
		texture->Init();
	}


}

void Result::Uninit()
{
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
	for (auto texture : m_Texture)
	{
		texture->Draw();
	}
}
