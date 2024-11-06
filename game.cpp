#include "game.h"
#include "input.h"
#include "manager.h"
#include "audio.h"
#include "result.h"
#include "particleEmitter.h"
#include "tree.h"
#include "predation.h"
#include "score.h"
#include "time.h"
#include "meshField.h"

void Game::Init()
{
	AddGameObject<Camera>(0);
	//AddGameObject<Field>(1);
	AddGameObject<MeshField>(1);
	AddGameObject<Player>(1);
	//AddGameObject<Enemy>(1)->SetPos(XMFLOAT3(0.0f, 0.0f, 20.0f));
	//AddGameObject<Enemy>(1)->SetPos(XMFLOAT3(10.0f, 0.0f, 20.0f));
	//AddGameObject<Enemy>(1)->SetPos(XMFLOAT3(-10.0f, 0.0f, 20.0f));

	{
		Cylinder* cylinder = AddGameObject<Cylinder>(1);
		cylinder->SetPos(XMFLOAT3(10.0f, 0.0f, -20.0f));
		cylinder->SetScale(XMFLOAT3(5.0f, 5.0f, 5.0f));
	}
	{
		Cylinder* cylinder = AddGameObject<Cylinder>(1);
		cylinder->SetPos(XMFLOAT3(-10.0f, 0.0f, -20.0f));
		cylinder->SetScale(XMFLOAT3(3.0f, 3.0f, 3.0f));
	}
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPos(XMFLOAT3(20.0f, 0.0f, -20.0f));
		box->SetScale(XMFLOAT3(3.0f, 3.0f, 3.0f));
	}
	AddGameObject<Sky>(1);
	//AddGameObject<PartrcleEmitter>(1)->SetPos(XMFLOAT3(5.0f, 0.0f, 5.0f));
	//AddGameObject<Tree>(1)->SetPos(XMFLOAT3(-10.0f, 0.0f, 0.0f));
	//AddGameObject<Polygon2D>(2);
	AddGameObject<Predation>(1);
	AddGameObject<Time>(2);

	//m_BGM = new Audio(this);
	//m_BGM->Load("asset\\audio\\gameBGM.wav");
	//m_BGM->Play(true);


}

void Game::Update()
{
	Scene::Update();
	std::vector<Enemy*> enemyList = GetGameObjects<Enemy>();
	//if (enemyList.size() == 0) {
	//	Manager::SetScene<Result>();
	//}
}

