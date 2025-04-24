#include "game.h"
#include <list>
#include <vector>
#include "input.h"
#include "manager.h"
#include "audio.h"
#include "gameObject.h"
#include "polygon2D.h"
#include "camera.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "sky.h"
#include "explosion.h"
#include "cylinder.h"
#include "box.h"
#include "result.h"
#include "failureResult.h"
#include "particleEmitter.h"
#include "tree.h"
#include "predation.h"
#include "score.h"
#include "time.h"
#include "meshField.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "buffParticle.h"
#include "wave.h"
#include "fade.h"
#include "lifeGauge.h"
#include "bulletGauge.h"
#include "staminaGauge.h"
#include "swordIcon.h"
#include "gunIcon.h"
#include "map.h"
#include "grass.h"
#include "building.h"
#include "aimIcon.h"

void Game::Init()
{

	AddGameObject<Camera>(0);
	//AddGameObject<Field>(1);
	AddGameObject<MeshField>(1);
	AddGameObject<Map>(1);
	AddGameObject<Building>(1)->GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(20.0f, 0.0f,-30.0f));
	AddGameObject<Player>(1)->GetComponent<Transform>()->SetPos(XMFLOAT3(0.0f, 0.5f, -50.0f));
	AddGameObject<Enemy>(1)->GetComponent<Transform>()->SetPos(XMFLOAT3(30.0f, 0.0f, 5.0f));
	AddGameObject<Polygon2D>(1);
	AddGameObject<Sky>(1);
	AddGameObject<BuffParticle>(1)->SetPlayerBuff(false);
	AddGameObject<Grass>(1)->SetPos(XMFLOAT3(50.0f,0.0f,-50.0f));

	AddUITexture<Time>()->SetTextureNum(0);
	AddUITexture<Fade>()->SetTextureNum(1);
	AddUITexture<LifeGauge>()->SetTextureNum(2);
	AddUITexture<BulletGauge>()->SetTextureNum(3);
	AddUITexture<StaminaGauge>()->SetTextureNum(4);
	AddUITexture<SwordIcon>()->SetTextureNum(5);
	AddUITexture<GunIcon>()->SetTextureNum(6);
	AddUITexture<AimIcon>()->SetTextureNum(7);
	GetUITexture<Fade>(1)->SetFade(false);

	m_BGM = new Audio();
	m_BGM->Load("asset\\audio\\gameBGM.wav");
	m_BGM->SetSound(0.0f);
	m_BGM->Play(true);

	m_Satate = SCENE_STATE::SCENE_GAME;

	m_ImGui = new ImguiManager;
	m_ImGui->Init();
}

void Game::Uninit()
{

	m_BGM->Uninit();
	delete m_BGM;

	for (int i = 0; i < LAYER_MAX; i++) {
		for (GameObject* object : m_GameObject[i]) {
			object->Uninit();
			delete object;
		}
		m_GameObject[i].clear();
	}
	for (auto ui : m_Texture)
	{
		ui->Uninit();
		delete ui;
	}

	m_ImGui->Uninit();
}

void Game::Update()
{
	for (int i = 0; i < LAYER_MAX; i++) {
		for (GameObject* object : m_GameObject[i]) {
			object->Update();
		}
		m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });	
	}
	for (auto ui : m_Texture)
	{
		ui->Update();
	}

	if (Input::GetKeyTrigger(VK_RETURN)) {
		Scene::GetInstance()->ChangeScene(new Result);
	}
	if (Input::GetKeyTrigger('I')) {
		if (!m_IsDrawNodeEditor) {
			m_IsDrawNodeEditor = true;
		}
		else {
			m_IsDrawNodeEditor = false;
		}
	}

	//クリアリザルト
	std::vector<Enemy*> enemyList = GetGameObjects<Enemy>();
	if (enemyList.size() == 0) {
		m_shoudNextScene = true;
	}

	if (m_shoudNextScene) m_TransitionCount++;

	if (m_TransitionCount >= 120)
	{
		m_TransitionCount = 0;
		m_shoudNextScene = false;
		m_IsClearResult = true;
		GetUITexture<Fade>(1)->SetFadeState(FADE_STATE::FADE_OUT);
	}

	if (m_IsClearResult)
	{
		m_ChageSceneFrame++;
		if (m_ChageSceneFrame >= 60)
		{
			GetUITexture<Fade>(1)->SetFade(true);
			if (GetUITexture<Fade>(1)->GetFadeState() == FADE_NONE)
			{
				Scene::GetInstance()->ChangeScene(new Result);
				return;
			}
		}
	}

	//失敗リザルト
	if (!m_IsFailureResult && GetGameObject<Player>()->GetPlayerDie())
	{
		m_IsFailureResult = true;
		GetUITexture<Fade>(1)->SetFadeState(FADE_STATE::FADE_OUT);
	}
	if (m_IsFailureResult)
	{
		m_ChageSceneFrame++;
		if (m_ChageSceneFrame >= 60)
		{
			GetUITexture<Fade>(1)->SetFade(true);
			if (GetUITexture<Fade>(1)->GetFadeState() == FADE_NONE)
			{
				Scene::GetInstance()->ChangeScene(new FailureResult);
				return;
			}
		}
	}

	if (Input::GetKeyTrigger('K')) {
		if (m_IsDrawColider) {
			m_IsDrawColider = false;
		}
		else {
			m_IsDrawColider = true;
		}
	}

	if (Input::GetKeyTrigger('O')) {
		if (m_IsDrawImGui) {
			m_IsDrawImGui = false;
		}
		else {
			m_IsDrawImGui = true;
		}
	}



}

void Game::Draw()
{
	Renderer::Begin();

	m_ImGui->ImGuiRendererInit();



	for (int i = 0; i < LAYER_MAX; i++) {
		//m_GameObject[i].sort();
		for (GameObject* object : m_GameObject[i]) {
			object->Draw();
		}
	}
	for (auto ui : m_Texture) {
		ui->Draw();
	}

	if (m_IsDrawImGui) {
		DrawImGui();
		m_ImGui->DrawNodeEditor();
	}
	m_ImGui->ImGuiRenderer();

	Renderer::End();
}

void Game::DrawImGui()
{
	{
		ImGui::Begin("Game");

		ImGui::Checkbox("IsDrawCollision", &m_IsDrawColider);		

		ImGui::End();
	}
}

