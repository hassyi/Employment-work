#include "manager.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "scene.h"

void Camera::Init()
{
	SetPos(XMFLOAT3(0.0f, 5.0f, -10.0f));
	SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetTarget(XMFLOAT3(0.0f, 0.0f, 0.0f));

	float vx, vy, vz;
	vx = GetPos().x - GetTarget().x;
	vy = GetPos().y - GetTarget().y;
	vz = GetPos().z - GetTarget().z;

	//m_len = sqrtf(vx * vx + vz * vz);

	m_R = sqrtf((vx * vx) + (vy * vy) + (vz * vz));
	m_S = atanf(vz / vx);
	m_F = atanf(sqrtf(vx * vx + vz * vz) / vy);
}

void Camera::Uninit()
{

}

void Camera::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	m_CameraCount++;
	m_Radian = XM_PI * 0.01f;

	//カメラ回転
	if (Input::GetKeyPress('Q'))
	{
		m_S += m_Radian;
		SetRotY(GetRot().y - m_Radian);
		m_FrameCoutnt[0] = 20.0f;
	}
	else if (m_FrameCoutnt[0] > 0)
	{
		m_Radian = XM_PI * (m_FrameCoutnt[0] * 0.0002f);
		m_S += m_Radian;
		SetRotY(GetRot().y - m_Radian);
		m_FrameCoutnt[0]--;
	}
	m_Radian = XM_PI * 0.01f;

	if (Input::GetKeyPress('E'))
	{
		m_S -= m_Radian;
		SetRotY(GetRot().y + m_Radian);
		m_FrameCoutnt[1] = 20.0f;
	}
	else if (m_FrameCoutnt[1] > 0)
	{
		m_Radian = XM_PI * (m_FrameCoutnt[1] * 0.0002f);
		m_S -= m_Radian;
		SetRotY(GetRot().y + m_Radian);
		m_FrameCoutnt[1]--;
	}
	m_Radian = XM_PI * 0.01f;

	if (GetPos().y <= (m_R - 3.0f) + player->GetPos().y)
	{
		if (Input::GetKeyPress('U'))
		{
			m_F -= m_Radian;
			m_FrameCoutnt[2] = 20.0f;
		}
		else if (m_FrameCoutnt[2] > 0)
		{
			m_Radian = XM_PI * (m_FrameCoutnt[2] * 0.0002f);
			m_F -= m_Radian;
			m_FrameCoutnt[2]--;
		}
	}
	m_Radian = XM_PI * 0.01f;

	if (GetPos().y >= (-m_R + 3.0f) + player->GetPos().y)
	{
		if (Input::GetKeyPress('J'))
		{
			m_F += m_Radian;
			m_FrameCoutnt[3] = 20.0f;
		}
		else if (m_FrameCoutnt[3] > 0)
		{
			m_Radian = XM_PI * (m_FrameCoutnt[3] * 0.0002f);
			m_F += m_Radian;
			m_FrameCoutnt[3]--;
		}
	}

	//カメラ切り替え(0:キー回転 1:マウス回転)
	if (Input::GetKeyTrigger(VK_TAB))
	{
		if (mouse == 0) {
			mouse = 1;
		}
		else if (mouse == 1) {
			mouse = 0;
		}
	}


}

void Camera::Draw()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	switch (mouse)
	{
	case 0:
		SetTarget(player->GetPos());

		SetPosX((m_R * sinf(m_F) * cosf(m_S)) + GetTarget().x);
		SetPosY((m_R * cosf(m_F)) + GetTarget().y);
		SetPosZ((m_R * sinf(m_F) * sinf(m_S)) + GetTarget().z);

		break;
	case 1:
		SetMouseCamera(player->GetPos());
		break;
	default:
		break;
	}


	//ビューマトリクス設定
	XMFLOAT3 up{ 0.0f,1.0f,0.0f };
	XMFLOAT3 camerapos = GetPos();
	XMFLOAT3 cameratarget = GetTarget();
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&camerapos), XMLoadFloat3(&cameratarget), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(viewMatrix);

	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix);
}

void Camera::SetMouseCamera(XMFLOAT3 pos)
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	m_OldmousePos = m_MousePos;

	m_MousePos = XMFLOAT2(GetMousePosX(), GetMousePosY());

	//中央から右にいる場合
	if (m_OldmousePos.x > m_CenterCamX)
	{
		if (m_MousePos.x > m_OldmousePos.x + 15.0f)
		{
			m_S -= m_Radian;
			SetRotY(GetRot().y + m_Radian);
			m_FrameCoutnt[1] = 20.0f;
		}
		else if (m_FrameCoutnt[1] > 0)
		{
			m_Radian = XM_PI * (m_FrameCoutnt[1] * 0.0002f);
			m_S -= m_Radian;
			SetRotY(GetRot().y + m_Radian);
			m_FrameCoutnt[1]--;
		}
		m_Radian = XM_PI * 0.01f;
	}
	//中央から左にいる場合
	if (m_OldmousePos.x < m_CenterCamX)
	{
		if (m_MousePos.x < m_OldmousePos.x - 15.0f)
		{
			m_S += m_Radian;
			SetRotY(GetRot().y - m_Radian);
			m_FrameCoutnt[0] = 20.0f;
		}
		else if (m_FrameCoutnt[0] > 0)
		{
			m_Radian = XM_PI * (m_FrameCoutnt[0] * 0.0002f);
			m_S += m_Radian;
			SetRotY(GetRot().y - m_Radian);
			m_FrameCoutnt[0]--;
		}
		m_Radian = XM_PI * 0.01f;
	}
	//中央から下にいる場合
	if (m_OldmousePos.y > m_CenterCamY)
	{
		if (GetPos().y <= (m_R - 3.0f) + player->GetPos().y)
		{
			if (m_MousePos.y > m_OldmousePos.y + 15.0f)
			{
				m_F -= m_Radian;
				m_FrameCoutnt[2] = 20.0f;
			}
			else if (m_FrameCoutnt[2] > 0)
			{
				m_Radian = XM_PI * (m_FrameCoutnt[2] * 0.0002f);
				m_F -= m_Radian;
				m_FrameCoutnt[2]--;
			}
		}
		m_Radian = XM_PI * 0.01f;
	}
	//中央から上にいる場合
	if (m_OldmousePos.y < m_CenterCamY)
	{
		if (GetPos().y >= (-m_R + 3.0f) + player->GetPos().y)
		{
			if (m_MousePos.y < m_OldmousePos.y - 15.0f)
			{
				m_F += m_Radian;
				m_FrameCoutnt[3] = 20.0f;
			}
			else if (m_FrameCoutnt[3] > 0)
			{
				m_Radian = XM_PI * (m_FrameCoutnt[3] * 0.0002f);
				m_F += m_Radian;
				m_FrameCoutnt[3]--;
			}
		}
		m_Radian = XM_PI * 0.01f;
	}

	SetTargetX(pos.x);
	SetTargetY(pos.y + m_len * 0.3f);
	SetTargetZ(pos.z);

	SetPosX((m_R * sinf(m_F) * cosf(m_S)) + GetTarget().x);
	SetPosY((m_R * cosf(m_F)) + GetTarget().y);
	SetPosZ((m_R * sinf(m_F) * sinf(m_S)) + GetTarget().z);

	//カーソルを中心に戻す
	if (m_CameraCount > 5)
	{
		SetCursorPos(m_CenterCamX, m_CenterCamY);
		m_CameraCount = 0;
	}
}
