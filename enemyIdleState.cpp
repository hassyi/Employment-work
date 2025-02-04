#include "enemyIdleState.h"
#include "transform3DAnimaitonComponent.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "enemyAIState.h"
#include "calculation.h"

void EnemyIdleState::Init()
{
	
}

void EnemyIdleState::Uninit()
{

}

void EnemyIdleState::Update(Enemy* enemy)
{
	XMFLOAT3 playerpos = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->GetPos();

	XMFLOAT3 pos = enemy->GetComponent<Transform>()->GetPos();

	enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");
	//プレイヤーまでのベクトル
	XMFLOAT3 vec = XMFLOAT3(playerpos.x - pos.x, playerpos.y - pos.y, playerpos.z - pos.z);
	vec = Normalize(vec);
	XMFLOAT3 FrontVec = enemy->GetComponent<Transform>()->GetForward();
	XMFLOAT3 RightVec = enemy->GetComponent<Transform>()->GetRight();

	{
		float dot = Dot(vec, FrontVec);

		float vec1len = Length(vec);
		float vec2len = Length(FrontVec);

		//ベクトルの大きさが0だったら処理を中止
		if (vec1len == 0 || vec2len == 0)
		{
			return;
		}

		// cosθを計算
		float cosTheta = dot / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acosf(cosTheta);
		float angleDegrees = angleRadians * 180.0f / XM_PI;

		//ほぼ正面を向いている
		if (angleDegrees < 5.0f)
		{
			enemy->GetComponent<EnemyAIState>()->ChangeState(STATE_CHASE);
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
			return;
		}

		// cosθを計算
		float cosTheta = dot / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acosf(cosTheta);
		float angleDegrees = angleRadians * 180.0f / XM_PI;

		if (angleDegrees < 90.0f)
		{
			XMFLOAT3 rot = enemy->GetComponent<Transform>()->GetRot();
			rot.y += 0.02f;
			enemy->GetComponent<Transform>()->SetRot(rot);
		}
		else
		{
			XMFLOAT3 rot = enemy->GetComponent<Transform>()->GetRot();
			rot.y -= 0.02f;
			enemy->GetComponent<Transform>()->SetRot(rot);
		}

	}



}

