#include "moveObject.h"
#include "input.h"

XMFLOAT3 PlayerMove::Move()
{
    SetVelX(0.0f);
    SetVelZ(0.0f);

	if (Input::GetKeyPress('W')) {
		SetVelX(GetVel().x + PLAYER_MOVE * cosf(pGameObject->GetRot().y));
		SetVelZ(GetVel().z + PLAYER_MOVE * sinf(pGameObject->GetRot().y));
	}
	else if (Input::GetKeyPress('A')) {
		SetVelX(GetVel().x + PLAYER_MOVE * cosf(pGameObject->GetRot().y));
		SetVelZ(GetVel().z - PLAYER_MOVE * sinf(pGameObject->GetRot().y));
	}
	else if (Input::GetKeyPress('S')) {
		SetVelX(GetVel().x - PLAYER_MOVE * cosf(pGameObject->GetRot().y));
		SetVelZ(GetVel().z - PLAYER_MOVE * sinf(pGameObject->GetRot().y));
	}
	else if (Input::GetKeyPress('D')) {
		SetVelX(GetVel().x - PLAYER_MOVE * cosf(pGameObject->GetRot().y));
		SetVelZ(GetVel().z + PLAYER_MOVE * sinf(pGameObject->GetRot().y));
	}


    return m_vel;
}
