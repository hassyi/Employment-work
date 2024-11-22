#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "audio.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "modelRenderer.h"


void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();

	Scene::GetInstance()->InitScene(new Game);

}

void Manager::Uninit()
{
	Scene::GetInstance()->Uninit();

	ModelRenderer::UnloadAll();

	Audio::UninitMaster();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();
	Scene::GetInstance()->Update();

}

void Manager::Draw()
{
	Renderer::Begin();
	Scene::GetInstance()->Draw();

	Renderer::End();
}
