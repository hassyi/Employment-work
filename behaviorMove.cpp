#include "behaviorMove.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "transform.h" 
#include "enemy.h"
#include "calculation.h"
#include "transform3DAnimaitonComponent.h"

BEHAVIOR_RESULT BehaviorMove::Update()
{
    m_BehaviorText = "Move";

    m_Enemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Walk");

    Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
    XMFLOAT3 playerpos = player->GetComponent<Transform>()->GetPos();
    XMFLOAT3 enemypos = m_Enemy->GetComponent<Transform>()->GetPos();
    m_Enemy->SetSpeed(0.05f);

    //プレイヤーとの距離をとる
    XMFLOAT3 direction{};
    direction.x = playerpos.x - enemypos.x;
    direction.y = playerpos.y - enemypos.y;
    direction.z = playerpos.z - enemypos.z;

    float length = Length(direction);
    direction = Normalize(direction);

    XMFLOAT3 FrontVec = m_Enemy->GetComponent<Transform>()->GetForward();
    XMFLOAT3 RightVec = m_Enemy->GetComponent<Transform>()->GetRight();
    {
        float dot = Dot(direction, RightVec);

        float vec1len = Length(direction);
        float vec2len = Length(RightVec);

        //ベクトルの大きさが0だったら処理を中止
        if (vec1len == 0 || vec2len == 0) {
            return BEHAVIOR_RESULT_CONTINUE;
        }

        // cosθを計算
        float cosTheta = dot / (vec1len * vec2len);

        cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

        float angleRadians = acosf(cosTheta);
        float angleDegrees = angleRadians * 180.0f / XM_PI;

        //正面を向いていない
        if (angleDegrees > 10.0f)
        {
            if (angleDegrees < 90.0f)
            {
                XMFLOAT3 rot = m_Enemy->GetComponent<Transform>()->GetRot();
                rot.y += 0.02f;
                m_Enemy->GetComponent<Transform>()->SetRot(rot);
                //return BEHAVIOR_RESULT_CONTINUE;
            }
            else
            {
                XMFLOAT3 rot = m_Enemy->GetComponent<Transform>()->GetRot();
                rot.y -= 0.02f;
                m_Enemy->GetComponent<Transform>()->SetRot(rot);
                //return BEHAVIOR_RESULT_CONTINUE;
            }
        }
    }

    if (length <= 3.5f)
    {
        m_Enemy->SetSpeed(0.0f);
        return BEHAVIOR_RESULT_SUCCESS;
    }

    enemypos.x += direction.x * m_Enemy->GetSpeed();
    enemypos.z += direction.z * m_Enemy->GetSpeed();
        
    m_Enemy->GetComponent<Transform>()->SetPos(enemypos);

    return BEHAVIOR_RESULT_CONTINUE;
}
