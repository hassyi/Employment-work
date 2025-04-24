#include "manager.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "game.h"
#include "transform3DComponent.h"
#include "aimIcon.h"

void Camera::Init()
{
	AddComponent<Transform3DComponent>();
	GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(0.0f, 5.0f, -10.0f));

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();

	float vx, vy, vz;
	vx = pos.x - m_Target.x;
	vy = pos.y - m_Target.y;
	vz = pos.z - m_Target.z;


	m_R = sqrtf((vx * vx) + (vy * vy) + (vz * vz));
	m_S = atanf(vz / vx);
	m_F = atanf(sqrtf(vx * vx + vz * vz) / vy);

	m_len = m_R;
	
	m_AimOffset = XMFLOAT3(0.5f, 1.8f, -3.0f);
	m_AimLookAt = XMFLOAT3(0.5f, 1.5f, 1.0f);

	m_ClientCenter = { m_ScreenWidthCamera / 2 , m_ScreenHeightCamera / 2 };
	ClientToScreen(GetWindow(), &m_ClientCenter);

}

void Camera::Uninit()
{

}

void Camera::Update()
{

	m_CameraCount++;
	m_Radian = XM_PI * 0.01f;

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

	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	if (player->GetWeaponType() == GUN) {
		m_IsAim = Input::GetKeyPress(MOUSEEVENTF_LEFTDOWN);			//LEFTDOWNだけど右クリック
	}

}

void Camera::Draw()
{
	DrawImGui();

	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	XMFLOAT3 playerpos = player->GetComponent<Transform>()->GetPos();

	m_Target = Add(playerpos, m_AddTarget);

	if (!m_IsAim)
	{
		Scene::GetInstance()->GetScene<Game>()->GetUITexture<AimIcon>(7)->SetIsDraw(false);
		m_AddTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_R = m_len;
		m_Target.y += 1.0f;

		switch (mouse)
		{
		case 0:
			SetKeyCamera();
			SetTarget(playerpos);

			GetComponent<Transform3DComponent>()->SetPosX((m_R * sinf(m_F) * cosf(m_S)) + m_Target.x);
			GetComponent<Transform3DComponent>()->SetPosY((m_R * cosf(m_F)) + m_Target.y);
			GetComponent<Transform3DComponent>()->SetPosZ((m_R * sinf(m_F) * sinf(m_S)) + m_Target.z);

			break;
		case 1:
			SetMouseCamera(player->GetComponent<Transform>()->GetPos());
			break;
		default:
			break;
		}

		//ビューマトリクス設定
		XMFLOAT3 up{ 0.0f,1.0f,0.0f };
		XMFLOAT3 camerapos = GetComponent<Transform3DComponent>()->GetPos();
		XMFLOAT3 cameratarget = m_Target;
		XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&camerapos), XMLoadFloat3(&cameratarget), XMLoadFloat3(&up));

		Renderer::SetViewMatrix(viewMatrix);

		XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

		//プロジェクションマトリクス設定
		XMMATRIX projectionMatrix;
		projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

		Renderer::SetProjectionMatrix(projectionMatrix);
	}
	else
	{
		AimCameraControl();
	}
}

void Camera::SetMouseCamera(XMFLOAT3 pos)
{
	XMFLOAT3 camerapos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 camerarot = GetComponent<Transform3DComponent>()->GetRot();

	m_OldmousePos = m_MousePos;

	m_MousePos = XMFLOAT2(GetMousePosX(), GetMousePosY());

	//中央から右にいる場合
	if (m_OldmousePos.x > m_CenterCamX)
	{
		if (m_MousePos.x > m_OldmousePos.x + 15.0f)
		{
			m_S -= m_Radian;
			GetComponent<Transform3DComponent>()->SetRotY(camerarot.y + m_Radian);
			m_FrameCoutnt[1] = 20.0f;
		}
		else if (m_FrameCoutnt[1] > 0)
		{
			m_Radian = XM_PI * (m_FrameCoutnt[1] * 0.0002f);
			m_S -= m_Radian;
			GetComponent<Transform3DComponent>()->SetRotY(camerarot.y + m_Radian);
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
			GetComponent<Transform3DComponent>()->SetRotY(camerarot.y - m_Radian);
			m_FrameCoutnt[0] = 20.0f;
		}
		else if (m_FrameCoutnt[0] > 0)
		{
			m_Radian = XM_PI * (m_FrameCoutnt[0] * 0.0002f);
			m_S += m_Radian;
			GetComponent<Transform3DComponent>()->SetRotY(camerarot.y - m_Radian);
			m_FrameCoutnt[0]--;
		}
		m_Radian = XM_PI * 0.01f;
	}
	//中央から下にいる場合
	if (m_OldmousePos.y > m_CenterCamY)
	{
		if (camerapos.y <= (m_R - 3.0f) + pos.y)
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
		if (camerapos.y >= (-m_R + 3.0f) + pos.y)
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

	SetTarget(pos);

	GetComponent<Transform3DComponent>()->SetPosX((m_R * sinf(m_F) * cosf(m_S)) + m_Target.x);
	GetComponent<Transform3DComponent>()->SetPosY((m_R * cosf(m_F)) + m_Target.y);
	GetComponent<Transform3DComponent>()->SetPosZ((m_R * sinf(m_F) * sinf(m_S)) + m_Target.z);

	//カーソルを中心に戻す
	if (m_CameraCount > 5)
	{
		SetCursorPos(m_ClientCenter.x, m_ClientCenter.y);
		m_CameraCount = 0;
	}
}

void Camera::SetKeyCamera()
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	XMFLOAT3 camerapos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 camerarot = GetComponent<Transform3DComponent>()->GetRot();

	//カメラ回転
	if (Input::GetKeyPress('Q'))
	{
		m_S += m_Radian;
		GetComponent<Transform3DComponent>()->SetRotY(camerarot.y - m_Radian);
		m_FrameCoutnt[0] = 20.0f;
	}
	else if (m_FrameCoutnt[0] > 0)
	{
		m_Radian = XM_PI * (m_FrameCoutnt[0] * 0.0002f);
		m_S += m_Radian;
		GetComponent<Transform3DComponent>()->SetRotY(camerarot.y - m_Radian);
		m_FrameCoutnt[0]--;
	}
	//m_Radian = XM_PI * 0.01f;

	else if (Input::GetKeyPress('E'))
	{
		m_S -= m_Radian;
		GetComponent<Transform3DComponent>()->SetRotY(camerarot.y + m_Radian);
		m_FrameCoutnt[1] = 20.0f;
	}
	else if (m_FrameCoutnt[1] > 0)
	{
		m_Radian = XM_PI * (m_FrameCoutnt[1] * 0.0002f);
		m_S -= m_Radian;
		GetComponent<Transform3DComponent>()->SetRotY(camerarot.y + m_Radian);
		m_FrameCoutnt[1]--;
	}
	m_Radian = XM_PI * 0.01f;

	if (camerapos.y <= (m_R - 3.0f) + player->GetComponent<Transform>()->GetPos().y)
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

	if (camerapos.y >= (-m_R + 3.0f) + player->GetComponent<Transform>()->GetPos().y)
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
}

void Camera::AimCameraControl()
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();

	XMFLOAT3 playerpos = player->GetComponent<Transform>()->GetPos();
	XMFLOAT3 playerrot = player->GetComponent<Transform>()->GetRot();
	XMFLOAT3 camerapos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 camerarot = GetComponent<Transform3DComponent>()->GetRot();
	XMFLOAT3 cameraRight = GetComponent<Transform>()->GetRight();

	Scene::GetInstance()->GetScene<Game>()->GetUITexture<AimIcon>(7)->SetIsDraw(true);

	//m_OldmousePos = m_MousePos;

	//m_MousePos = XMFLOAT2(GetMousePosX(), GetMousePosY());
	float mouseSensitivity = 0.002f;
	float yaw = 0.0f, pitch = 0.0f;
	float interpolationSpeed = 0.1f;

	//カメラ回転(キーボード)
	if(mouse ==0)
	{
		if (Input::GetKeyPress('Q'))
		{
			m_S += m_Radian;
			GetComponent<Transform3DComponent>()->SetRotY(camerarot.y - m_Radian);
			m_FrameCoutnt[0] = 20.0f;
		}
		else if (m_FrameCoutnt[0] > 0)
		{
			m_Radian = XM_PI * (m_FrameCoutnt[0] * 0.0002f);
			m_S += m_Radian;
			GetComponent<Transform3DComponent>()->SetRotY(camerarot.y - m_Radian);
			m_FrameCoutnt[0]--;
		}
		//m_Radian = XM_PI * 0.01f;

		else if (Input::GetKeyPress('E'))
		{
			m_S -= m_Radian;
			GetComponent<Transform3DComponent>()->SetRotY(camerarot.y + m_Radian);
			m_FrameCoutnt[1] = 20.0f;
		}
		else if (m_FrameCoutnt[1] > 0)
		{
			m_Radian = XM_PI * (m_FrameCoutnt[1] * 0.0002f);
			m_S -= m_Radian;
			GetComponent<Transform3DComponent>()->SetRotY(camerarot.y + m_Radian);
			m_FrameCoutnt[1]--;
		}
		m_Radian = XM_PI * 0.01f;
	}
	//カメラ回転(マウス)
	else 
	{
		m_OldmousePos = m_MousePos;

		m_MousePos = XMFLOAT2(GetMousePosX(), GetMousePosY());

		//中央から右にいる場合
		if (m_OldmousePos.x > m_CenterCamX)
		{
			if (m_MousePos.x > m_OldmousePos.x + 15.0f)
			{
				m_S -= m_Radian;
				GetComponent<Transform3DComponent>()->SetRotY(camerarot.y + m_Radian);
				m_FrameCoutnt[1] = 20.0f;
			}
			else if (m_FrameCoutnt[1] > 0)
			{
				m_Radian = XM_PI * (m_FrameCoutnt[1] * 0.0002f);
				m_S -= m_Radian;
				GetComponent<Transform3DComponent>()->SetRotY(camerarot.y + m_Radian);
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
				GetComponent<Transform3DComponent>()->SetRotY(camerarot.y - m_Radian);
				m_FrameCoutnt[0] = 20.0f;
			}
			else if (m_FrameCoutnt[0] > 0)
			{
				m_Radian = XM_PI * (m_FrameCoutnt[0] * 0.0002f);
				m_S += m_Radian;
				GetComponent<Transform3DComponent>()->SetRotY(camerarot.y - m_Radian);
				m_FrameCoutnt[0]--;
			}
			m_Radian = XM_PI * 0.01f;
		}

	}

	m_AddTarget = XMFLOAT3(cameraRight.x * 1.0f, cameraRight.y * -1.5f, cameraRight.z * 1.0f);

	if (m_R >= 3.5f) {
		m_R -= 0.2f;
	}

	playerrot = XMFLOAT3(playerrot.x, camerarot.y, playerrot.z);
	player->GetComponent<Transform>()->SetRot(playerrot);

	XMFLOAT3 playerForward = player->GetComponent<Transform>()->GetForward();

	GetComponent<Transform3DComponent>()->SetPosX((m_R * sinf(m_F) * cosf(m_S)) + m_Target.x);
	GetComponent<Transform3DComponent>()->SetPosY((m_R * cosf(m_F)) + m_Target.y);
	GetComponent<Transform3DComponent>()->SetPosZ((m_R * sinf(m_F) * sinf(m_S)) + m_Target.z);

	XMFLOAT3 up{ 0.0f,1.0f,0.0f };
	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
	XMVECTOR cameratarget = XMLoadFloat3(&playerpos) + XMLoadFloat3(&playerForward) * 5.0f;
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&pos), cameratarget, XMLoadFloat3(&up));

	Renderer::SetViewMatrix(viewMatrix);

	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix);


	//カーソルを中心に戻す
	if (m_CameraCount > 5)
	{
		SetCursorPos(m_ClientCenter.x, m_ClientCenter.y);
		m_CameraCount = 0;
	}

}

//視錐台カリング・ビューフラスタムカリング
bool Camera::CheckView(XMFLOAT3 pos, float size)
{
	//ビューマトリクス設定
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };
	XMFLOAT3 camerapos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 cameraTarget = m_Target;
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&camerapos), XMLoadFloat3(&cameraTarget), XMLoadFloat3(&up));

	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f);

	XMMATRIX vp, invvp;
	XMVECTOR det;

	vp = viewMatrix * projectionMatrix;
	invvp = XMMatrixInverse(&det, vp);

	//空間上の座標を求める
	XMFLOAT3 vpos[4];
	XMFLOAT3 wpos[4];

	vpos[0] = { -1.0f,  1.0f,1.0f };
	vpos[1] = {  1.0f,  1.0f,1.0f };
	vpos[2] = { -1.0f, -1.0f,1.0f };
	vpos[3] = {  1.0f, -1.0f,1.0f };

	XMVECTOR vposv[4];
	XMVECTOR wposv[4];

	vposv[0] = XMLoadFloat3(&vpos[0]);
	vposv[1] = XMLoadFloat3(&vpos[1]);
	vposv[2] = XMLoadFloat3(&vpos[2]);
	vposv[3] = XMLoadFloat3(&vpos[3]);

	wposv[0] = XMVector3TransformCoord(vposv[0], invvp);
	wposv[1] = XMVector3TransformCoord(vposv[1], invvp);
	wposv[2] = XMVector3TransformCoord(vposv[2], invvp);
	wposv[3] = XMVector3TransformCoord(vposv[3], invvp);

	XMStoreFloat3(&wpos[0], wposv[0]);
	XMStoreFloat3(&wpos[1], wposv[1]);
	XMStoreFloat3(&wpos[2], wposv[2]);
	XMStoreFloat3(&wpos[3], wposv[3]);

	XMFLOAT3 v, v1, v2, n;

	v = Sub(pos, camerapos);

	//左面判定
	{
		v1 = Sub(wpos[0], camerapos);
		v2 = Sub(wpos[2], camerapos);

		//外積
		n = Cross(v1, v2);

		//nの正規化
		n = Normalize(n);

		//内積
		float dot = Dot(n, v);

		if (dot < -size) return false;
	}

	//右面判定
	{
		v1 = Sub(wpos[3], camerapos);
		v2 = Sub(wpos[1], camerapos);

		//外積
		n = Cross(v1, v2);

		//nの正規化
		n = Normalize(n);

		//内積
		float dot = Dot(n, v);

		if (dot < -size) return false;
	}

	//上面判定
	{
		v1 = Sub(wpos[1], camerapos);
		v2 = Sub(wpos[0], camerapos);

		//外積
		n = Cross(v1, v2);

		//nの正規化
		n = Normalize(n);

		//内積
		float dot = Dot(n, v);

		if (dot < -size) return false;
	}

	//下面判定
	{
		v1 = Sub(wpos[2], camerapos);
		v2 = Sub(wpos[3], camerapos);

		//外積
		n = Cross(v1, v2);

		//nの正規化
		n = Normalize(n);

		//内積
		float dot = Dot(n, v);

		if (dot < -size) return false;
	}

	return true;
}

void Camera::DrawImGui()
{
	XMFLOAT3 transPos = GetComponent<Transform>()->GetPos();
	XMFLOAT3 transRot = GetComponent<Transform>()->GetRot();

	{
		ImGui::Begin("Camera");

		ImGui::Text("CameraPos : x = %.1f, y = %.1f, z = %.1f", transPos.x, transPos.y, transPos.z);
		ImGui::Text("CameraRot : x = %.1f, y = %.1f, z = %.1f", transRot.x, transRot.y, transRot.z);

		ImGui::End();
	}
}
