#include "behaviorDodge.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "transform3DAnimaitonComponent.h"

BEHAVIOR_RESULT BehaviorDodge::Update()
{
	m_BehaviorText = "Dodge";

	XMFLOAT3 enemyPos = m_Enemy->GetComponent<Transform>()->GetPos();
	XMFLOAT3 enemyRot = m_Enemy->GetComponent<Transform>()->GetRot();
	XMFLOAT3 dodgeVel{};
	float dodgeSpeed = -0.2f;

	if (!m_isDodge)
	{
		m_Enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationFrame(0);
		m_isDodge = true;
		m_DodgeFrame = 80.0f;
		m_Enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Jump");
	}
	else
	{
		m_DodgeFrame -= 1.0f;
	}

	if (m_DodgeFrame <= 50.0f)
	{
		dodgeVel = XMFLOAT3(sinf(enemyRot.y) * dodgeSpeed, dodgeVel.y, cosf(enemyRot.y) * dodgeSpeed);
	}

	if (m_DodgeFrame <= 0)
	{
		m_isDodge = false;
		return BEHAVIOR_RESULT_SUCCESS;
	}

	enemyPos.x += dodgeVel.x;
	enemyPos.z += dodgeVel.z;

	m_Enemy->GetComponent<Transform>()->SetPos(enemyPos);

	return BEHAVIOR_RESULT_CONTINUE;
}
