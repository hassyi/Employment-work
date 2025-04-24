#include "behaviorSelector.h"


BEHAVIOR_RESULT BehaviorSelector::Update()
{
    BEHAVIOR_RESULT restult = m_Child[m_Index]->Update();

    switch (restult)
    {
    case BEHAVIOR_RESULT_CONTINUE:
        return BEHAVIOR_RESULT_CONTINUE;

    case BEHAVIOR_RESULT_SUCCESS:
        m_Index = 0;
        return BEHAVIOR_RESULT_SUCCESS;
        m_IsSuccess = true;

    case BEHAVIOR_RESULT_FAILURE:
        m_Index++;
        if (m_Index < m_Child.size())
        {
            return BEHAVIOR_RESULT_CONTINUE;
        }
        else
        {
            m_Index = 0;
            return BEHAVIOR_RESULT_FAILURE;
        }

    }

}
