#include "behaviorIdle.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "transform.h" 
#include "enemy.h"
#include "calculation.h"
#include "transform3DAnimaitonComponent.h"

BEHAVIOR_RESULT BehaviorIdle::Update()
{
    m_BehaviorText = "Idle";

    m_Enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");

    Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
    XMFLOAT3 playerpos = player->GetComponent<Transform>()->GetPos();
    XMFLOAT3 enemypos = m_Enemy->GetComponent<Transform>()->GetPos();
    m_Enemy->SetSpeed(0.0f);

    XMFLOAT3 direction{};
    direction.x = playerpos.x - enemypos.x;
    direction.y = playerpos.y - enemypos.y;
    direction.z = playerpos.z - enemypos.z;

    float length = Length(direction);

    if (length < 20.0f)    {
        return BEHAVIOR_RESULT_SUCCESS;
    }

    return BEHAVIOR_RESULT_CONTINUE;
    
}
