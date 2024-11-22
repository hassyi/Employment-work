#include "main.h"
#include "title.h"
#include "input.h"
#include "manager.h"
#include "game.h"
#include "texture2D.h"

void Title::Init()
{
	AddUITexture<Texture2D>()->SetTransTexNum(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, L"asset\\texture\\title.png", 1);

	for (auto texture : m_Texture)
	{
		texture->Init();
	}

}

void Title::Uninit()
{
	for (auto texture : m_Texture)
	{
		texture->Uninit();
		delete texture;
	}
}

void Title::Update()
{
	for (auto texture : m_Texture)
	{
		texture->Update();
	}
	if (Input::GetKeyTrigger(VK_RETURN)) {
		Scene::GetInstance()->ChangeScene(new Game);
	}
}

void Title::Draw()
{
	for (auto texture : m_Texture)
	{
		texture->Draw();
	}
}
