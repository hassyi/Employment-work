#include "enemyDodgeState.h"
#include "transform.h"
#include "scene.h"
#include "game.h"
#include "enemy.h"
#include "player.h"
#include "calculation.h"
#include "enemyAIState.h"


void EnemyDodgeState::Init()
{
}

void EnemyDodgeState::Uninit()
{
}

void EnemyDodgeState::Update(Enemy* enemy)
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	XMFLOAT3 playerPos = player->GetComponent<Transform>()->GetPos();
	XMFLOAT3 enemyPos = enemy->GetComponent<Transform>()->GetPos();
	XMFLOAT3 enemyRot = enemy->GetComponent<Transform>()->GetRot();
	XMFLOAT3 dodgeVel{};
	float dodgeSpeed = 0.5f;

	if (!m_isDodge)
	{
		m_isDodge = true;
		m_DodgeFrame = 90;
	}
	else
	{
		m_DodgeFrame--;
		dodgeVel = XMFLOAT3(cosf(enemyRot.y) * dodgeSpeed, dodgeVel.y, sinf(enemyRot.y) * dodgeSpeed);
	}

	if (m_DodgeFrame <= 0)
	{
		m_DodgeFrame = 0;
		m_isDodge = false;
		enemy->GetComponent<EnemyAIState>()->ChangeState(STATE_CHASE);
	}

	enemyPos.x -= dodgeVel.x;
	enemyPos.z -= dodgeVel.z;

	enemy->GetComponent<Transform>()->SetPos(dodgeVel);

}
