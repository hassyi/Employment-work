#include "player.h"
#include "animationModel.h"
#include "input.h"
#include "camera.h"
#include "scene.h"
#include "manager.h"
#include "bullet.h"
#include "collision.h"
#include "cylinder.h"
#include "title.h"
#include "result.h"
#include "audio.h"
#include "predation.h"
#include "meshField.h"

void Player::Init()
{
	SetPos(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));

	m_Component = new AnimationModel(this);
	((AnimationModel*)m_Component)->Load("asset\\model\\Vampire A Lusth.fbx");
	((AnimationModel*)m_Component)->LoadAnimation("asset\\model\\Idle.fbx", "Idle");
	((AnimationModel*)m_Component)->LoadAnimation("asset\\model\\Running.fbx", "Run");

	m_AnimationName1 = "Idle";
	m_AnimationName2 = "Idle";

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_SE = new Audio(this);
	m_SE->Load("asset\\audio\\shot.wav");

	m_Scale = { 0.01f,0.01f,0.01f };

}

void Player::Uninit()
{
	m_SE->Uninit();
	delete m_SE;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	Scene* scene = Manager::GetScene();
	Predation* predation = scene->GetGameObject<Predation>();

	float dt = 1.0f / 60.0f;
	m_GroundHeight = 0;

	SetOldPos(GetPos());
	m_AnimationName = "Idle";

	if (Input::GetKeyPress(VK_LSHIFT) || predation->GetBuff()) {
		m_move = 2.0f;
	}
	else {
		m_move = 1.0f;
	}

	SetPosX(GetPos().x + Move().x * dt);
	SetPosZ(GetPos().z + Move().z * dt);

	//ジャンプ
	if (Input::GetKeyPress(VK_SPACE)) 
	{
		if (!m_JampFlag) 
		{
			SetVelY(20.0f);
			m_JampFlag = true;
		}
	}


	SetVelX(GetVel().x * 50.0f * dt);
	SetVelZ(GetVel().z * 50.0f * dt);
	SetVelY(GetVel().y + m_Gravity * dt);		//重力加速度
	if (m_IsGravity)
	{
		m_Gravity = -30.0f;

	}
	if (!m_IsGravity)
	{
		SetPosY(GetOldPos().y);
		SetVelY(0.0f);
		m_Gravity = 0.0f;
		m_JampFlag = false;
	}

	SetPosY(GetPos().y + GetVel().y * dt);

	PlayerCollision();

	if (GetPos().y < m_GroundHeight) 
	{
		SetPosY(m_GroundHeight);
		SetVelY(0.0f);
		m_JampFlag = false;
		m_IsGravity = false;
	}
	else
	{
		m_IsGravity = true;
	}


	if (Input::GetKeyTrigger('F')) {
		m_dir.x = cosf(GetRot().y - (XM_PI / 2));
		m_dir.y = 0.0f;
		m_dir.z = sinf(GetRot().y + (XM_PI / 2));
		Bullet* bullet = scene->AddGameObject<Bullet>(1);
		bullet->SetPos(GetPos());
		m_SE->Play();
	}
	if (Input::GetKeyTrigger(VK_RETURN)) 
	{
		Manager::SetScene<Result>();
	}
	PredationAttack();

	if (m_AnimationName2 != m_AnimationName)
	{
		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = m_AnimationName;
		m_AnimetionBlendRatio = 0.0f;
	}
	m_AnimetionBlendRatio += 0.1f;
	if (m_AnimetionBlendRatio > 1.0f) {
		m_AnimetionBlendRatio = 1.0f;
	}

}

void Player::Draw()
{
	((AnimationModel*)m_Component)->Update(m_AnimationName1.c_str(), m_AnimationFrame,
											m_AnimationName2.c_str(), m_AnimationFrame,
											m_AnimetionBlendRatio);
	m_AnimationFrame++;

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRot().x, GetRot().y, GetRot().z);
	trans = XMMatrixTranslation(GetPos().x, GetPos().y, GetPos().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	m_Component->Draw();
}

XMFLOAT3 Player::Move()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	SetVelX(GetVel().x);
	SetVelZ(GetVel().z);


	if (Input::GetKeyPress('W')) 
	{
		SetVelX(GetVel().x + m_move * sinf(camera->GetRot().y));
		SetVelZ(GetVel().z + m_move * cosf(camera->GetRot().y));
		m_Rot = 0.0f;
		m_AnimationName = "Run";
	}
	if (Input::GetKeyPress('A')) 
	{
		SetVelX(GetVel().x - m_move * cosf(camera->GetRot().y));
		SetVelZ(GetVel().z + m_move * sinf(camera->GetRot().y));
		m_Rot = -3.14f / 2.0f;
		m_AnimationName = "Run";
	}
	if (Input::GetKeyPress('S')) 
	{
		SetVelX(GetVel().x - m_move * sinf(camera->GetRot().y));
		SetVelZ(GetVel().z - m_move * cosf(camera->GetRot().y));
		m_Rot = 3.14f;
		m_AnimationName = "Run";
	}
	if (Input::GetKeyPress('D')) 
	{
		SetVelX(GetVel().x + m_move * cosf(camera->GetRot().y));
		SetVelZ(GetVel().z - m_move * sinf(camera->GetRot().y));
		m_Rot = 3.14f / 2.0f;
		m_AnimationName = "Run";
	}

	SetRotY(m_Rot + camera->GetRot().y);

	return GetVel();
}

void Player::PlayerCollision()
{
	Scene* scene = Manager::GetScene();
	Collision collision;

	//メッシュフィールド高さ取得
	MeshField* meshField = scene->GetGameObject<MeshField>();

	//地面の高さ
	m_GroundHeight = meshField->GetHeight(GetPos());

	auto cylinderList = scene->GetGameObjects<Cylinder>();

	for (auto cylinder : cylinderList) {
		XMFLOAT3 cylinderPos = cylinder->GetPos();
		XMFLOAT3 cylinderScale = cylinder->GetScale();

		if (collision.CollisionCylinder(GetPos(), cylinderPos, cylinderScale.x)) 
		{
			if (collision.CollisionCylinderHeight(GetPos(), cylinderPos, cylinderScale.y)) {
				m_GroundHeight = cylinderPos.y + cylinderScale.y;
			}
			else 
			{
				SetPosX(GetOldPos().x);
				SetPosZ(GetOldPos().z);
				SetVelX(0.0f);
				SetVelZ(0.0f);
			}
		}
	}

	float posY = GetPos().y - GetScale().y;

	if (std::get<0>(collision.CollisionBB(GetPos(), GetScale())))
	{
		std::list <Box*> BoxList = std::get<1>(collision.CollisionBB(GetPos(), GetScale()));
		for (auto box : BoxList)
		{
			float boxposy = box->GetPos().y + box->GetScale().y;
			if (posY >= boxposy + (box->GetScale().y - 1.0f))
			{
				m_GroundHeight = boxposy + box->GetScale().y;
				m_Gravity = 0.0f;
				m_JampFlag = false;
			}
			else
			{
				SetPosX(GetOldPos().x);
				SetPosZ(GetOldPos().z);
				SetVelX(0.0f);
				SetVelZ(0.0f);
			}
		}

	}
}

void Player::PredationAttack()
{
	Scene* scene = Manager::GetScene();
	Predation* predation = scene->GetGameObject<Predation>();

	XMFLOAT3 predationPos;
	predationPos.x = GetPos().x + (GetScale().x * 2.0f) * cosf(GetRot().y - (XM_PI / 2));
	predationPos.z = GetPos().z + (GetScale().z * 2.0f) * sinf(GetRot().y + (XM_PI / 2));
	predationPos.y = GetPos().y;

	if (Input::GetKeyTrigger('R')) 
	{
		predation->SetUse(true);
		predation->SetPos(predationPos);
		SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}

}
