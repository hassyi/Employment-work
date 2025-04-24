#include "behaviorGun.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "transform3DComponent.h"
#include "enemyBullet.h"
#include "calculation.h"

BEHAVIOR_RESULT BehaviorGun::Update()
{
	m_BehaviorText = "Gun";

	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	XMFLOAT3 playerpos = player->GetComponent<Transform>()->GetPos();
	XMFLOAT3 enemypos = m_Enemy->GetComponent<Transform>()->GetPos();

	XMFLOAT3 direction{};

	direction.x = playerpos.x - enemypos.x;
	direction.x = playerpos.y - enemypos.y;
	direction.x = playerpos.z - enemypos.z;

	float length = Length(direction);

	if (length >= 2.0f)
	{
		m_ShotFrame++;
		if (m_ShotFrame >= 20)
		{
			m_Enemy->SetSpeed(0.0f);
			EnemyBullet* bullet = Scene::GetInstance()->GetScene<Game>()->AddGameObject<EnemyBullet>(1);
			bullet->GetComponent<Transform3DComponent>()->SetPos(enemypos);
			return BEHAVIOR_RESULT_SUCCESS;
		}
	}
	else if (length < 2.0f) {
		return BEHAVIOR_RESULT_FAILURE;
	}

}
