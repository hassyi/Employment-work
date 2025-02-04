#include "behaviorSequence.h"

BEHAVIOR_RESULT BehaviorSequence::Update()
{
    BEHAVIOR_RESULT result = m_Child[m_Index]->Update();

	switch (result)
	{
	case BEHAVIOR_RESULT_CONTINUE:
		return BEHAVIOR_RESULT_CONTINUE;

	case BEHAVIOR_RESULT_SUCCESS:
		m_Index++;
		if (m_Index < m_Child.size())
		{
			return BEHAVIOR_RESULT_CONTINUE;
		}
		else
		{
			m_Index = 0;
			return BEHAVIOR_RESULT_SUCCESS;
		}

	case BEHAVIOR_RESULT_FAILURE:
		m_Index = 0;
		return BEHAVIOR_RESULT_FAILURE;
	}

}
