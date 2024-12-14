#include "searchPlayerState.h"
#include <random>

void SearchPlayerState::Init()
{
	m_State = SearchState::Idle;
}

void SearchPlayerState::Uninit()
{

}

void SearchPlayerState::Update()
{
	XMFLOAT3 pos;

	if (m_State == SearchState::FindPos)
	{
		pos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;

		m_State = SearchState::Walk;
	}
	else if (m_State == SearchState::Walk)
	{

	}
	else if (m_State == SearchState::Idle)
	{

	}



}

