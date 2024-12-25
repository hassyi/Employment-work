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
#include "particleEmitter.h"
#include "tree.h"
#include "predation.h"
#include "score.h"
#include "time.h"
#include "meshField.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "buffParticle.h"
#include "cloth.h"
#include "wave.h"
#include "fade.h"
#include "lifeGauge.h"
#include "bulletGauge.h"
#include "staminaGauge.h"

void Game::Init()
{
	AddGameObject<Camera>(0);
	//AddGameObject<Field>(1);
	AddGameObject<MeshField>(1);
	//AddGameObject<Enemy>(1)->GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(10.0f, 0.0f, 10.0f));
	//AddGameObject<Enemy>(1)->GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(10.0f, 0.0f, -10.0f));

	//{
	//	AddGameObject<Cylinder>(1);
	//	GetGameObject<Cylinder>()->GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(10.0f, 0.0f, 10.0f));
	//	GetGameObject<Cylinder>()->GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
	//	GetGameObject<Cylinder>()->GetComponent<Transform3DComponent>()->SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//}
	{
		Box* box = AddGameObject<Box>(1);
		box->GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(0.0f, 2.0f, 10.0f));
		box->GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
		box->GetComponent<Transform3DComponent>()->SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
	{
		Box* box = AddGameObject<Box>(1);
		box->GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(0.0f, 2.0f, -10.0f));
		box->GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
		box->GetComponent<Transform3DComponent>()->SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}

	AddGameObject<Player>(1);
	AddGameObject<Enemy>(1)->GetComponent<Transform>()->SetPos(XMFLOAT3(10.0f, 0.0f, 5.0f));
	AddGameObject<Polygon2D>(1);
	//AddGameObject<Cloth>(1);
	AddGameObject<Sky>(1);
	//AddGameObject<PartrcleEmitter>(1)->GetComponent<Transform2DComponent>()->SetPos(XMFLOAT3(5.0f, 0.0f, 5.0f));
	AddGameObject<BuffParticle>(1)->SetPlayerBuff(false);
	//AddGameObject<Tree>(1)->GetComponent<Transform2DComponent>()->SetPos(XMFLOAT3(-10.0f, 0.0f, 0.0f));

	//MeshField* meshField = GetGameObject<MeshField>();
	//for (int i = 0; i < 10; i++)
	//{
	//	Tree* tree = AddGameObject<Tree>(1);

	//	XMFLOAT3 pos;
	//	pos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
	//	pos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
	//	pos.y = meshField->GetHeight(pos);

	//	tree->GetComponent<Transform2DComponent>()->SetPos(pos);
	//}
	AddGameObject<Predation>(1);
	//AddGameObject<Wave>(1);
	//AddUITexture<Score>();
	AddUITexture<Time>()->SetTextureNum(0);
	AddUITexture<LifeGauge>()->SetTextureNum(2);
	AddUITexture<BulletGauge>()->SetTextureNum(3);
	AddUITexture<StaminaGauge>()->SetTextureNum(4);
	AddUITexture<Fade>()->SetTextureNum(1);
	GetUITexture<Fade>(1)->SetFade(false);

	//m_BGM = new Audio(this);
	//m_BGM->Load("asset\\audio\\gameBGM.wav");
	//m_BGM->Play(true);

	m_Satate = SCENE_STATE::SCENE_GAME;
}

void Game::Uninit()
{
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
	//std::vector<Enemy*> enemyList = GetGameObjects<Enemy>();
	//if (enemyList.size() == 0) {
	//Scene::GetInstance()->ChangeScene(new Result);
	//}
}

void Game::Draw()
{
	Renderer::Begin();

	for (int i = 0; i < LAYER_MAX; i++) {
		//m_GameObject[i].sort();
		for (GameObject* object : m_GameObject[i]) {
			object->Draw();
		}
	}
	for (auto ui : m_Texture)
	{
		ui->Draw();
	}
	Renderer::End();
}

