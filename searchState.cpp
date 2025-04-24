#include "searchState.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "calculation.h"
#include "transform3DAnimaitonComponent.h"


void SearchState::Init()
{

}

void SearchState::Uninit()
{

}

void SearchState::Update()
{
	XMFLOAT3 playerpos = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPos();

	XMFLOAT3 enemypos = m_Enemy->GetComponent<Transform>()->GetPos();

	m_Enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");

	//プレイヤーまでのベクトル
	XMFLOAT3 vec = XMFLOAT3(playerpos.x - enemypos.x, playerpos.y - enemypos.y, playerpos.z - enemypos.z);
	vec = Normalize(vec);
	XMFLOAT3 FrontVec = m_Enemy->GetComponent<Transform>()->GetForward();
	XMFLOAT3 RightVec = m_Enemy->GetComponent<Transform>()->GetRight();
	{
		float dot = Dot(vec, FrontVec);

		float vec1len = Length(vec);
		float vec2len = Length(FrontVec);

		//ベクトルの大きさが0だったら処理を中止
		if (vec1len == 0 || vec2len == 0)
		{
			return ;
		}

		// cosθを計算
		float cosTheta = dot / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acosf(cosTheta);
		m_AngleDegrees = angleRadians * 180.0f / XM_PI;


		//ほぼ正面を向いている
		if (m_AngleDegrees < 10.0f) {
			m_Enemy->GetComponent<EnemyAIState>()->ChangeState(ENEMY_STATE::STATE_BATTLE);
			return;
		}
	}

	{
		float dot = Dot(vec, RightVec);

		float vec1len = Length(vec);
		float vec2len = Length(RightVec);

		//ベクトルの大きさが0だったら処理を中止
		if (vec1len == 0 || vec2len == 0)
		{
			return ;
		}

		// cosθを計算
		float cosTheta = dot / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acosf(cosTheta);
		m_AngleDegrees = angleRadians * 180.0f / XM_PI;

		if (m_AngleDegrees < 90.0f)
		{
			XMFLOAT3 rot = m_Enemy->GetComponent<Transform>()->GetRot();
			rot.y += 0.02f;
			m_Enemy->GetComponent<Transform>()->SetRot(rot);
		}
		else
		{
			XMFLOAT3 rot = m_Enemy->GetComponent<Transform>()->GetRot();
			rot.y -= 0.02f;
			m_Enemy->GetComponent<Transform>()->SetRot(rot);
		}
	}
	return ;

}
