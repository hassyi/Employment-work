#include "titleCamera.h"
#include "player.h"
#include "scene.h"
#include "title.h"
#include "transform.h"

void TitleCamera::Init()
{
	SetPos(XMFLOAT3(0.0f, 1.2f, -8.0f));
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

void TitleCamera::Uninit()
{

}

void TitleCamera::Update()
{

}

void TitleCamera::Draw()
{
	Player* player = Scene::GetInstance()->GetScene<Title>()->GetGameObject<Player>();

	SetTarget(player->GetComponent<Transform>()->GetPos());

	SetPosX((m_R * sinf(m_F) * cosf(m_S)) + GetTarget().x);
	SetPosY((m_R * cosf(m_F)) + GetTarget().y);
	SetPosZ((m_R * sinf(m_F) * sinf(m_S)) + GetTarget().z);

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
