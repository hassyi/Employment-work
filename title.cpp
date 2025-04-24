#include "main.h"
#include "title.h"
#include "input.h"
#include "manager.h"
#include "game.h"
#include "texture2D.h"
#include "player.h"
#include "titleCamera.h"
#include "field.h"
#include "sky.h"
#include "fieldDepthShadow.h"
#include "wave.h"
#include "polygon2D.h"
#include "bonFire.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "box.h"
#include "fireParticle.h"
#include "titleName.h"
#include "fade.h"

void Title::Init()
{
	m_Satate = SCENE_STATE::SCENE_TITLE;
	AddGameObject<TitleCamera>(0);
	AddGameObject<Field>(1)->SetPosZ(-45.0f);
	AddGameObject<Polygon2D>(1);
	AddGameObject<Player>(1);
	AddGameObject<Wave>(1)->SetPos(XMFLOAT3(-50.0f, -3.0f, 90.0f));
	AddGameObject<BonFire>(1)->GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(2.0f, 0.0f, 0.0f));
	AddGameObject<Sky>(1);
	AddGameObject<FireParticle>(1);
	AddUITexture<TitleName>()->SetTextureNum(0);
	AddUITexture<Texture2D>()->SetTransTexNum(480.0f, 550.0f, 350.0f, 40.0f, L"asset\\texture\\start.png", 1);
	AddUITexture<Fade>()->SetTextureNum(2);
	GetUITexture<Fade>(2)->SetFade(false);

	for (auto texture : m_Texture)
	{
		texture->Init();
	}

	m_BGM = new Audio();
	m_BGM->Load("asset\\audio\\title1.wav");
	m_BGM->SetSound(0.5f);
	m_BGM->Play(true);
	m_SE = new Audio();
	m_SE->Load("asset\\audio\\start.wav");
}

void Title::Uninit()
{
	m_BGM->Uninit();
	delete m_BGM;

	m_SE->Uninit();
	delete m_SE;

	for (int i = 0; i < LAYER_MAX; i++) {
		for (GameObject* object : m_GameObject[i]) {
			object->Uninit();
			delete object;
		}
		m_GameObject[i].clear();
	}

	for (auto texture : m_Texture)
	{
		texture->Uninit();
		delete texture;
	}
}

void Title::Update()
{
	for (int i = 0; i < LAYER_MAX; i++) {
		for (GameObject* object : m_GameObject[i]) {
			object->Update();
		}
		m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });
	}

	for (auto texture : m_Texture)
	{
		texture->Update();
	}

	m_DrawFrame++;

	if (m_DrawFrame >= 30) {
		GetUITexture<Texture2D>(1)->SetIsDraw(false);
	}
	if (m_DrawFrame >= 60) {
		GetUITexture<Texture2D>(1)->SetIsDraw(true);
		m_DrawFrame = 0;
	}

	if (Input::GetKeyTrigger(VK_RETURN)) {
		GetUITexture<Texture2D>(1)->SetIsDraw(true);
		m_SE->Play();
		m_isChageScene = true;
		GetUITexture<Fade>(2)->SetFadeState(FADE_STATE::FADE_OUT);
	}

	if (m_isChageScene)
	{
		m_ChageSceneFrame++;
		if (m_ChageSceneFrame >= 60)
		{
			GetUITexture<Fade>(2)->SetFade(true);
			if (GetUITexture<Fade>(2)->GetFadeState() == FADE_NONE)
			{
				Scene::GetInstance()->ChangeScene(new Game);
			}
		}
	}
}

void Title::Draw()
{

	Renderer::Begin();

	for (int i = 0; i < LAYER_MAX; i++) {
		//m_GameObject[i].sort();
		for (GameObject* object : m_GameObject[i]) {
			object->Draw();
		}
	}

	for (auto texture : m_Texture)
	{
		texture->Draw();
	}

	Renderer::End();


}
