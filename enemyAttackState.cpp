#include "enemyAttackState.h"
#include "transform3DAnimaitonComponent.h"
#include "enemyAIState.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "calculation.h"

void EnemyAttackState::Init()
{

}

void EnemyAttackState::Uninit()
{

}

void EnemyAttackState::Update(Enemy* enemy)
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	XMFLOAT3 playerpos = player->GetComponent<Transform>()->GetPos();
	XMFLOAT3 enemypos = enemy->GetComponent<Transform>()->GetPos();

	XMFLOAT3 direction{};

	direction.x = playerpos.x - enemypos.x;
	direction.x = playerpos.y - enemypos.y;
	direction.x = playerpos.z - enemypos.z;

	float length = Length(direction);

	if (m_isAttack) m_AttackFrame--;
	if (!m_isAttack && enemy->GetSpeed() == 0.0f)
	{
		enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationFrame(0);
		m_isAttack = true;
		m_AttackFrame = 80;
		enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Attack");
	}

	if (m_AttackFrame <= 0 && length > 3.5f) {
		m_isAttack = false;
		m_AttackFrame = 0;
		enemy->GetComponent<EnemyAIState>()->ChangeState(STATE_DODGE);
	}
}