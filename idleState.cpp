#include "idleState.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "calculation.h"
#include "transform3DAnimaitonComponent.h"


void IdleState::Init()
{
}

void IdleState::Uninit()
{
}

void IdleState::Update()
{
	m_Enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");

	XMFLOAT3 playerpos = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPos();

	XMFLOAT3 enemypos = m_Enemy->GetComponent<Transform>()->GetPos();
	m_Enemy->SetSpeed(0.0f);

	XMFLOAT3 direction = XMFLOAT3(playerpos.x - enemypos.x, playerpos.y - enemypos.y, playerpos.z - enemypos.z);
	float length = Length(direction);

	if (length <= 20.0f) {
		m_Enemy->GetComponent<EnemyAIState>()->ChangeState(ENEMY_STATE::STATE_SEARCH);
	}


}
