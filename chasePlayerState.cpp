#include "chasePlayerState.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "transform3DAnimaitonComponent.h"
#include "enemyAIState.h"
#include "calculation.h"


void ChasePlayerState::Init()
{
	m_State = CHASE_STATE::IDLE;
}

void ChasePlayerState::Uninit()
{
}

void ChasePlayerState::Update(Enemy* enemy)
{
	XMFLOAT3 playerpos = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPos();

	XMFLOAT3 pos = enemy->GetComponent<Transform>()->GetPos();

	//プレイヤーまでのベクトル
	XMFLOAT3 vec = XMFLOAT3(playerpos.x - pos.x, playerpos.y - pos.y, playerpos.z - pos.z);
	vec = Normalize(vec);
	XMFLOAT3 FrontVec = enemy->GetComponent<Transform>()->GetForward();
	XMFLOAT3 RightVec = enemy->GetComponent<Transform>()->GetRight();

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
	m_AngleDegrees = angleRadians * 180.0f / XM_PI;

	//ほぼ正面を向いている
	if (m_AngleDegrees < 10.0f){
		m_State = CHASE_STATE::CHASE;
	}
	else{
		m_State = CHASE_STATE::IDLE;
	}

	if (m_State == CHASE_STATE::IDLE)
	{
		enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");
				
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
		m_AngleDegrees = angleRadians * 180.0f / XM_PI;

		if (m_AngleDegrees < 90.0f)
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
	else if (m_State == CHASE_STATE::CHASE)
	{

		XMFLOAT3 direction{};
		direction.x = playerpos.x - pos.x;
		direction.y = playerpos.y - pos.y;
		direction.z = playerpos.z - pos.z;

		float length = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		direction.x /= length;
		direction.y /= length;
		direction.z /= length;

		if (length >= 3.5f)
		{
			enemy->SetSpeed(0.02f);
			enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Walk");
		}
		else
		{
			enemy->GetComponent<EnemyAIState>()->ChangeState(STATE_ATTACK);
			enemy->SetSpeed(0.0f);
		}

		pos.x += direction.x * enemy->GetSpeed();
		pos.z += direction.z * enemy->GetSpeed();

		enemy->GetComponent<Transform>()->SetPos(pos);
	}
}

