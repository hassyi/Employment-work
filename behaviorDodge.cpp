#include "behaviorDodge.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "transform.h"

BEHAVIOR_RESULT BehaviorDodge::Update()
{
	m_BehaviorText = "Dodge";

	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	XMFLOAT3 playerPos = player->GetComponent<Transform>()->GetPos();
	XMFLOAT3 enemyPos = m_Enemy->GetComponent<Transform>()->GetPos();
	XMFLOAT3 enemyRot = m_Enemy->GetComponent<Transform>()->GetRot();
	XMFLOAT3 dodgeVel{};
	float dodgeSpeed = 0.2f;

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
		return BEHAVIOR_RESULT_SUCCESS;
	}

	enemyPos.x -= dodgeVel.x;
	enemyPos.z -= dodgeVel.z;

	m_Enemy->GetComponent<Transform>()->SetPos(dodgeVel);

	return BEHAVIOR_RESULT_CONTINUE;
}
