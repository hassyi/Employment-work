#include "enemy.h"
#include "scene.h"
#include "game.h"
#include "modelRenderer.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "transform3DAnimaitonComponent.h"
#include "boxColiderComponent.h"
#include "capsuleColiderComponent.h"
#include "meshField.h"
#include <random>
#include "player.h"
#include "sphereShadow.h"
#include <iostream>
#include <algorithm>
#include "enemyAIState.h"
#include "enemyAI.h"
#include "enemyDeathParticle.h"
#include <queue>
#include "unordered_set"

void Enemy::Init()
{
	AddComponent<Transform3DAnimationComponent>()->AddModelData("asset\\model\\Maw J Laygo.fbx", this);
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\EnemyIdle.fbx", "Idle");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\EnemyWalking.fbx", "Walk");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\EnemyAttack.fbx", "Attack");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\Dying.fbx", "Dying");
	GetComponent<Transform3DAnimationComponent>()->SetInitAnimationState("Idle");

	AddComponent<Transform>();
	AddComponent<CapsuleColiderComponent>();
	//AddComponent<EnemyAIState>()->SetEnemy(this);
	//AddComponent<EnemyAI>()->SetNode(this);
	GetComponent<Transform>()->SetScale(XMFLOAT3(0.02f, 0.02f, 0.02f));
	AddComponent<SphereShadow>()->SetScale(XMFLOAT3(1.5f, 1.5f, 1.5f));

	SetLife(30.0f);

	m_ObjType = OBJ_TYPE::ENEMY;

	for (auto component : m_ComponentList)
	{
		component->Init();
	}

	//GetComponent<BoxColiderComponent>()->SetScale(XMFLOAT3(1.0f, 2.0f, 1.0f));
	GetComponent<CapsuleColiderComponent>()->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
	GetComponent<CapsuleColiderComponent>()->SetSegmentLength(1.0f);
	GetComponent<Colider>()->SetAddPos(XMFLOAT3(0.0f, 2.0f, 0.0f));

	m_Speed = 0.05f;
}

void Enemy::Uninit()
{
	//GetComponent<EnemyAIState>()->Uninit();

	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Enemy::Update()
{

	if (GetLife() <= 0.0f)
	{
		DeathAnim();
		return;
	}

	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	if (player->GetIsDie()) {
		return;
	}

	EnemyCollision();

	//GetComponent<EnemyAIState>()->SetEnemy(this);
	//GetComponent<EnemyAI>()->Update();

	XMFLOAT3 pos = GetComponent<Transform>()->GetPos();

	if (m_IsGravity) 
	{
		m_Gravity = -10.0f;
		pos.y += m_Gravity;
	}

	if (pos.y < m_GroundHeight) {
		pos.y = m_GroundHeight;
		m_IsGravity = false;
		m_Gravity = 0.0f;
	}
	else {
		m_IsGravity = true;
	}

	std::vector<int> path = AStar(waypoints, 0, 2);
	MoveAI(pos, waypoints, path, m_Speed);

	GetComponent<Transform>()->SetPos(pos);
	GetComponent<SphereShadow>()->SetPos(pos);
		
	for (auto component : m_ComponentList)
	{
		component->Update();
	}

}

void Enemy::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}

	if (Scene::GetInstance()->GetScene<Game>()->GetIsDrawImGui()) {
		DrawImGui();
	}
}

void Enemy::EnemyCollision()
{

	MeshField* meshField = Scene::GetInstance()->GetScene<Game>()->GetGameObject<MeshField>();
	//地面の高さ
	m_GroundHeight = meshField->GetHeight(GetComponent<Transform>()->GetPos());

	XMFLOAT3 pos = GetComponent<Transform>()->GetPos();
	XMFLOAT3 scale = GetComponent<Transform>()->GetScale();
	XMFLOAT3 vel = GetComponent<Transform>()->GetVel();
	//GetColider()->SetPos(pos);
	XMFLOAT3 coliderPos = GetColider()->GetPos();
	XMFLOAT3 coliderScale = GetColider()->GetScale();

	std::tuple<bool, GameObject*, std::list<GameObject*>> objectList = GetComponent<Colider>()->GetCollision();


	if (std::get<0>(objectList))
	{
		if (pos.y <= m_GroundHeight)
		{
			pos.y = m_GroundHeight;
			m_IsGravity = false;
			GetComponent<Transform>()->SetPosY(pos.y);
		}
		else {
			m_Gravity = true;
		}
	}
	else
	{
		//地面の高さ
		m_GroundHeight = meshField->GetHeight(pos);

		if (pos.y <= m_GroundHeight)
		{
			pos.y = m_GroundHeight;
			m_IsGravity = false;
		}
		else {
			m_Gravity = true;
		}
		GetComponent<Transform>()->SetPos(pos);
		GetColider()->SetPos(pos);
	}


}

void Enemy::DeathAnim()
{
	if (m_IsDethAnim) m_DyingFrame--;
	if (!m_IsDethAnim && !m_IsDie)
	{
		GetComponent<Transform3DAnimationComponent>()->SetAnimationFrame(0);
		GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Dying");
		m_IsDethAnim = true;
		m_DyingFrame = 104;
	}

	if (m_DyingFrame <= 0 && !m_IsDie)
	{
		m_IsDie = true;
		m_IsDethAnim = false;
		m_DyingFrame = 0;
		GetComponent<Transform3DAnimationComponent>()->SetAddAnimFrame(0);
		XMFLOAT3 pos = GetComponent<Transform>()->GetPos();
		EnemyDeathParticle* particle = Scene::GetInstance()->GetScene<Game>()->AddGameObject<EnemyDeathParticle>(1);
		particle->GetComponent<Transform2DComponent>()->SetPos(pos);
	}

	if (m_IsDie) m_DieFrame++;

	if (m_DieFrame >= 120) {
		EnemyDeathParticle* particle = Scene::GetInstance()->GetScene<Game>()->GetGameObject<EnemyDeathParticle>();
		particle->SetDestroy();
		SetDestroy();
	}
}

void Enemy::MoveAI(XMFLOAT3& pos, const std::vector<WayPoint>& waypoints, const std::vector<int> path, float speed)
{
	static int currentTarget = 1;
	if (currentTarget >= path.size()) return;

	XMFLOAT3 targetPos = waypoints[path[currentTarget]].pos;
	XMFLOAT3 direction = Sub(targetPos, pos);

	float length = Length(direction);
	direction = Normalize(direction);

	pos.x += direction.x * speed;
	pos.z += direction.z * speed;

	if (length < 0.1f) {
		currentTarget++;
	}
}

std::vector<int> Enemy::AStar(const std::vector<WayPoint>& waypoints, int startID, int goalID)
{
	std::priority_queue < Node, std::vector<Node>, std::greater<Node> > openList;
	std::unordered_map<int, Node> allNodes;
	std::unordered_set<int> closedSet;

	openList.push({ startID, 0, Heuristic(waypoints[startID].pos, waypoints[goalID].pos), -1 });
	allNodes[startID] = { startID, 0, Heuristic(waypoints[startID].pos, waypoints[goalID].pos), -1 };

	while (!openList.empty())
	{
		Node current = openList.top();
		openList.pop();

		if (closedSet.count(current.Id)) continue;
		closedSet.insert(current.Id);

		//ゴールに到達したら経路を復元
		if (current.Id == goalID)
		{
			std::vector<int> path;
			for (int at = goalID; at != -1; at = allNodes[at].parent)
			{
				path.push_back(at);
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		//隣接ノードを処理
		for (int neighborID : waypoints[current.Id].neighbors)
		{
			if (closedSet.count(neighborID)) continue;

			float newG = current.gCost + Heuristic(waypoints[current.Id].pos, waypoints[neighborID].pos);
			if (!allNodes.count(neighborID) || newG < allNodes[neighborID].gCost)
			{
				allNodes[neighborID] = { neighborID, newG, Heuristic(waypoints[neighborID].pos, waypoints[goalID].pos), current.Id };
				openList.push(allNodes[neighborID]);
			}
		}
	}
	return{};
}

std::vector<Node2*> Enemy::AStar(Node2* start, Node2* goal, const std::vector<std::vector<bool>>& grid)
{
	std::vector<Node2*> openSet, closedSet;
	openSet.push_back(start);

	while (!openSet.empty())
	{
		// f(n)が最小のノードを探す
		auto current = *std::min_element(openSet.begin(), openSet.end(), [](Node2* a, Node2* b) {
			return a->fCost() < b->fCost();
			});

		// ゴールに到達したか
		if (current->x == goal->x && current->y == goal->y)
		{
			// 経路復元
			std::vector<Node2*> path;
			while (current != nullptr)
			{
				path.push_back(current);
				current = current->parentNode;
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		// openListから削除してclosedListに追加
		openSet.erase(std::remove(openSet.begin(), openSet.end(), current), openSet.end());
		closedSet.push_back(current);

		//近傍ノード(上下左右＋斜め)を確認
		for (int dx = -1; dx <= 1; ++dx)
		{
			for (int dy = -1; dy <= 1; ++dy)
			{
				if (dx == 0 && dy == 0)continue;	//自分自身を除外
				int nx = current->x + dx;
				int ny = current->y + dy;

				//範囲外チェック
				if (nx < 0 || ny < 0 || nx >= grid.size() || ny >= grid[0].size()) continue;

				//通行不能（壁）ならスキップ
				if (!grid[nx][ny]) continue;

				//すでにclosedListにあるか確認
				bool isClosed = std::any_of(closedSet.begin(), closedSet.end(), [&](Node2* n) {
					return n->x == nx && n->y == ny;
					});
				if (isClosed) continue;

				//新しいノード作成
				Node2* neighbor = new Node2{ nx, ny, 0, 0.0f, 0.0f, 0, nullptr };

				//すでopenSetにあるか？
				if (std::find_if(closedSet.begin(), closedSet.end(), [&](Node2* n) {return n->x == nx && n->y == ny; }) != closedSet.end()) continue;

				//移動コスト計算(斜め：1.4f, 縦横：1.0f)
				float tentativeG = current->gCost + ((dx == 0 || dy == 0) ? 1.0f : 1.4f);
				
				bool inOpen = false;
				for (auto& n : openSet)
				{
					if (n->x == nx && n->y == ny)
					{
						inOpen = true;
						if (tentativeG < n->gCost)
						{
							//既存ノードより良ければ更新
							n->gCost = tentativeG;
							n->parentNode = current;
						}
						break;
					}
				}

				if (!inOpen)
				{
					//新規ノードとして追加
					neighbor->gCost = tentativeG;
					neighbor->hCost = std::abs(nx - goal->x) + std::abs(ny - goal->y);	//マンハッタン距離
					neighbor->parentNode = current;
					openSet.push_back(neighbor);
				}

			}

		}

	}

	return {};
}

void Enemy::DrawImGui()
{
	XMFLOAT3 transPos = GetComponent<Transform>()->GetPos();
	XMFLOAT3 transRot = GetComponent<Transform>()->GetRot();
	XMFLOAT3 coliderPos = GetColider()->GetPos();
	XMFLOAT3 coliderRot = GetColider()->GetRot();

	{
		ImGui::Begin("Enemy");

		ImGui::Text("This is some useful text.");
		ImGui::Text("EnemyPos : x = %.1f, y = %.1f, z = %.1f", transPos.x, transPos.y, transPos.z);
		ImGui::Text("EnemyRot : x = %.1f, y = %.1f, z = %.1f", transRot.x, transRot.y, transRot.z);
		ImGui::Text("Life : %f", GetLife());

		ImGui::End();
	}


}
