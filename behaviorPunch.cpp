#include "behaviorPunch.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "transform3DAnimaitonComponent.h"
#include "calculation.h"
#include "enemySlash.h"

BEHAVIOR_RESULT BehaviorPunch::Update()
{
	m_BehaviorText = "Punch";

	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	XMFLOAT3 playerpos = player->GetComponent<Transform>()->GetPos();
	XMFLOAT3 enemypos = m_Enemy->GetComponent<Transform>()->GetPos();

	XMFLOAT3 direction{};

	direction.x = playerpos.x - enemypos.x;
	direction.x = playerpos.y - enemypos.y;
	direction.x = playerpos.z - enemypos.z;

	float length = Length(direction);

	if (m_isAttack) m_AttackFrame-=0.5f;
	if (!m_isAttack && m_Enemy->GetSpeed() == 0.0f)
	{
		m_Enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationFrame(0);
		m_isAttack = true;
		m_AttackFrame = 40;
		m_Enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Attack");
	}

	if (m_AttackFrame == 20) Scene::GetInstance()->GetScene<Game>()->AddGameObject<EnemySlash>(1);

	if (m_AttackFrame < 0.0f && m_isAttack) 
	{
		m_isAttack = false;
		m_AttackFrame = 0.0f;
		m_AttackCount++;
	}

	if (m_AttackCount >= 2) 
	{
		m_AttackCount = 0;
		return BEHAVIOR_RESULT_SUCCESS;
	}

	if (length > 4.0f) 
	{
		m_AttackCount = 0;
		return BEHAVIOR_RESULT_FAILURE;
	}
}
