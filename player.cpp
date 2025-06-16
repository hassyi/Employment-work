#include "player.h"
#include "input.h"
#include "camera.h"
#include "scene.h"
#include "bullet.h"
#include "title.h"
#include "result.h"
#include "failureResult.h"
#include "audio.h"
#include "predation.h"
#include "meshField.h"
#include "transform3DComponent.h"
#include "transform3DAnimaitonComponent.h"
#include "transform2DComponent.h"
#include "game.h"
#include "boxColiderComponent.h"
#include "capsuleColiderComponent.h"
#include "buffParticle.h"
#include "sphereShadow.h"
#include "enemy.h"
#include "sword.h"
#include "slash.h"


void Player::Init()
{
	SCENE_STATE scene = Scene::GetInstance()->GetNowScene()->GetSceneState();

	m_TransAnim = AddComponent<Transform3DAnimationComponent>();
	m_TransAnim->AddModelData("asset\\model\\Vampire A Lusth.fbx", this);
	if (scene == SCENE_STATE::SCENE_TITLE) {		//タイトルシーン用の読み込み
		GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\Idle_Title.fbx", "TitleIdle");
		GetComponent<Transform3DAnimationComponent>()->SetInitAnimationState("TitleIdle");
		GetComponent<Transform3DAnimationComponent>()->Init();
		AddComponent<Transform>();
		GetComponent<Transform>()->SetScale(XMFLOAT3(0.01f, 0.01f, 0.01f));
		GetComponent<Transform>()->Init();
		return;
	}
	m_TransAnim->AddAnimationData("asset\\model\\Idle.fbx", "Idle");
	m_TransAnim->AddAnimationData("asset\\model\\Running.fbx", "Run");
	m_TransAnim->AddAnimationData("asset\\model\\Attack1.fbx", "Attack1");
	m_TransAnim->AddAnimationData("asset\\model\\Attack2.fbx", "Attack2");
	m_TransAnim->AddAnimationData("asset\\model\\predationAttack.fbx", "PredationAttack");
	m_TransAnim->AddAnimationData("asset\\model\\Dying.fbx", "Dying");
	m_TransAnim->SetInitAnimationState("Idle");
	m_TransAnim->SetVSName("shader\\toonVS.cso");
	m_TransAnim->SetPSName("shader\\toonPS.cso");

	m_TransAnim->SetScale(XMFLOAT3(0.01f, 0.01f, 0.01f));
	AddComponent<CapsuleColiderComponent>();
	AddComponent<SphereShadow>()->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));

	for (auto component : m_ComponentList)
	{
		component->Init();
	}

	m_Life = 20.0f;
	m_BulletPoint = BULLET_POINT_MAX;

	m_ObjType = OBJ_TYPE::PLAYER;
	m_SE = new Audio(this);
	m_SE->Load("asset\\audio\\shot.wav");

	GetComponent<CapsuleColiderComponent>()->SetScale(XMFLOAT3(0.5f, 1.0f, 0.5f));
	GetComponent<CapsuleColiderComponent>()->SetSegmentLength(1.0f);
	GetComponent<CapsuleColiderComponent>()->SetAddPos(XMFLOAT3(0.0f, 1.0f, 0.0f));

}

void Player::Uninit()
{
	if (m_SE != nullptr) {
		m_SE->Uninit();
		delete m_SE;
	}

	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Player::Update()
{
	//タイトルシーンのプレイヤー
	SCENE_STATE scene = Scene::GetInstance()->GetNowScene()->GetSceneState();
	if (scene == SCENE_STATE::SCENE_TITLE)
	{
		m_TransAnim->Update();
		return;
	}

	if (m_Life <= 0.0f)
	{
		DeathAnim();
		return;
	}

	m_Predation = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Predation>();

	PlayerControl();
	PlayerCollision();

	GetComponent<SphereShadow>()->SetPos(m_TransAnim->GetPos());
	if (m_BulletPoint >= BULLET_POINT_MAX) {
		m_BulletPoint = BULLET_POINT_MAX;
	}

	for (auto component : m_ComponentList)
	{
		component->Update();
	}

}

void Player::Draw()
{
	for (auto component : m_ComponentList)
	{
		component->Draw();
	}

	SCENE_STATE scene = Scene::GetInstance()->GetNowScene()->GetSceneState();
	if (scene == SCENE_STATE::SCENE_TITLE)
	{
		return;
	}

	if (Scene::GetInstance()->GetScene<Game>()->GetIsDrawImGui()) {
		DrawImGui();
	}
}

//操作関連
void Player::PlayerControl()
{
	Camera* camera = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Camera>();

	XMFLOAT3 pos = GetComponent<Transform>()->GetPos();
	XMFLOAT3 vel = GetComponent<Transform>()->GetVel();
	XMFLOAT3 rot = GetComponent<Transform>()->GetRot();
	XMFLOAT3 camerarot = camera->GetComponent<Transform3DComponent>()->GetRot();

	float dt = 30.0f * (1.0f / 60.0f);
	float dt1 = 1.0f / 60.0f;

	//ダッシュ
	if (Input::GetKeyPress(VK_LSHIFT) || m_IsBuff) {
		PlayerBuff();
		m_move = 0.6f;
		m_StepSpeed = 5.0f;
	}
	else {
		m_move = 0.3f;
		m_StepSpeed = 3.0f;
	}

	//移動
	if (Input::GetKeyPress('W')) {
		m_Rot = 0.0f;
	}
	if (Input::GetKeyPress('A')) {
		m_Rot = -XM_PI / 2.0f;
	}
	if (Input::GetKeyPress('S')) {
		m_Rot = XM_PI;
	}
	if (Input::GetKeyPress('D')) {
		m_Rot = XM_PI / 2.0f;
	}

	if (m_AttackFrame <= 0 && m_SecondAttackFrame <= 0 && m_PredationFrame <= 0)
	{
		if (Input::GetKeyPress('W') || Input::GetKeyPress('A') || Input::GetKeyPress('S') || Input::GetKeyPress('D'))
		{
			rot = XMFLOAT3(rot.x, m_Rot + camerarot.y, rot.z);
			vel = XMFLOAT3(sinf(rot.y) * m_move, vel.y, cosf(rot.y) * m_move);
			m_TransAnim->SetAnimationState("Run");
		}
		else {
			m_TransAnim->SetAnimationState("Idle");
		}
	}

	//バフ中のアニメーションの加速
	if (m_IsBuff) {
		m_TransAnim->SetAddAnimFrame(2);
	}
	else {
		m_TransAnim->SetAddAnimFrame(1);
	}

	//ステップ
	{
		if (Input::GetKeyTrigger('G'))
		{
			if (!m_isStep && !m_JampFlag)
			{
				m_isStep = true;
				m_isAttackCancel = true;
				m_StepFrame = 10;
			}
		}

		if (m_isStep)
		{
			m_StepFrame--;
			vel = XMFLOAT3(sinf(rot.y) * m_StepSpeed, vel.y, cosf(rot.y) * m_StepSpeed);
		}

		if (m_StepFrame <= 0)
		{
			m_isStep = false;
			m_StepFrame = 0;
		}
	}

	//武器切り替え
	if (Input::GetKeyTrigger('C'))
	{
		if (m_Weapon == SWORD) {
			m_Weapon = GUN;
		}
		else {
			m_Weapon = SWORD;
		}
	}

	PlayerAttack();

	//重力判定
	if (m_IsGravity) {
		m_Gravity -= 1.0f;
		m_JampFlag = true;
	}
	else
	{
		m_Gravity = 0.0f;
		m_JampFlag = false;
	}

	vel.x *= dt;
	vel.z *= dt;
	vel.y += m_Gravity * dt1;

	//ジャンプ
	if (Input::GetKeyTrigger(VK_SPACE))
	{
		if (!m_JampFlag)
		{
			vel.y = 15.0f;
			m_JampFlag = true;
		}
	}

	m_TransAnim->SetVel(vel);
	m_TransAnim->SetRot(rot);
	m_TransAnim->SetPos(XMFLOAT3(pos.x + vel.x * dt, pos.y + vel.y * dt1, pos.z + vel.z * dt));
	GetColider()->SetPos(pos);
}

//当たり判定
void Player::PlayerCollision()
{
	Colider* colider = GetComponent<Colider>();

	XMFLOAT3 pos = m_TransAnim->GetPos();
	XMFLOAT3 scale = m_TransAnim->GetScale();
	XMFLOAT3 vel = m_TransAnim->GetVel();
	XMFLOAT3 coliderPos = colider->GetPos();
	XMFLOAT3 coliderScale = colider->GetScale();

	std::tuple<bool, GameObject*, std::list<GameObject*>> objectList = colider->GetCollision();

	if (std::get<0>(objectList))
	{

		if (pos.y <= m_GroundHeight)
		{
			pos.y = m_GroundHeight;
			m_IsGravity = false;
			m_JampFlag = false;
			m_TransAnim->SetPosY(pos.y);
		}
		if (m_JampFlag) {
			m_IsGravity = true;
		}
	}
	else
	{
		//地面の高さ
		m_GroundHeight = Scene::GetInstance()->GetScene<Game>()->GetGameObject<MeshField>()->GetHeight(pos);

		if (m_JampFlag)
		{
			m_IsGravity = true;
		}

		if (pos.y <= m_GroundHeight)
		{
			pos.y = m_GroundHeight;
			m_IsGravity = false;
		}

		m_TransAnim->SetPos(pos);
		colider->SetPos(pos);

	}
}

//捕食攻撃
void Player::PredationAttack()
{
	XMFLOAT3 pos = m_TransAnim->GetPos();
	XMFLOAT3 rot = m_TransAnim->GetRot();

	XMFLOAT3 predationPos;
	predationPos.x = pos.x + 2.0f * cosf(rot.y - (XM_PI / 2));
	predationPos.z = pos.z + 2.0f * sinf(rot.y + (XM_PI / 2));
	predationPos.y = pos.y;

	m_Predation = Scene::GetInstance()->GetScene<Game>()->AddGameObject<Predation>(1);
	m_Predation->GetComponent<Transform3DComponent>()->SetPos(predationPos);
	m_TransAnim->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
}

//バフのパーティクル
void Player::PlayerBuff()
{
	m_BuffParticle = Scene::GetInstance()->GetScene<Game>()->GetGameObject<BuffParticle>();
	if (m_BuffParticle == nullptr) return;

	XMFLOAT3 pos = m_TransAnim->GetPos();

	m_BuffParticle->SetPlayerBuff(m_IsBuff);
	m_BuffParticle->GetComponent<Transform2DComponent>()->SetPos(pos);
}

//プレイヤーの攻撃
void Player::PlayerAttack()
{
	Enemy* enemy = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Enemy>();

	//剣モード
	if (m_Weapon == SWORD)
	{
		//1回目の攻撃処理
		if (m_isAttack) m_AttackFrame--;
		if (!m_isAttack && !m_isSecondAttack)
		{
			if (Input::GetKeyTrigger('F') || Input::GetKeyTrigger(MOUSEEVENTF_MOVE))		//MOUSEEVENTF_MOVE←これがなぜか左クリック
			{
				m_TransAnim->SetAnimationFrame(0);
				m_isAttack = true;
				m_TransAnim->SetAnimationState("Attack1");
				if (!m_IsBuff) {
					m_AttackFrame = 72;
				}
				else {
					m_AttackFrame = 36;
				}
			}
		}
		//ここで当たり判定をつける
		if (m_AttackFrame == 25) {
			Scene::GetInstance()->GetScene<Game>()->AddGameObject<Slash>(1)->SetRot(XMFLOAT3(0.0f, 0.0f, -1.8f));
		}
		if (m_AttackFrame == 10)
		{
			m_isNext = true;
			m_AttackReseveFrame = 60;
		}

		//次の攻撃の受付時間
		if (m_isNext)
		{
			m_AttackReseveFrame--;
			if (m_AttackReseveFrame <= 0)
			{
				m_isNext = false;
				m_isAttack = false;
				m_AttackFrame = 0;
			}
			else if (Input::GetKeyTrigger('F') || Input::GetKeyTrigger(MOUSEEVENTF_MOVE))
			{
				m_isNextOnAttack = true;
				m_isNext = false;
				m_AttackReseveFrame = 0;
			}
		}

		//２回目の攻撃処理
		if (m_isSecondAttack) m_SecondAttackFrame--;

		if (m_SecondAttackFrame == 30) {
			Scene::GetInstance()->GetScene<Game>()->AddGameObject<Slash>(1)->SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));
		}

		if (m_AttackFrame <= 0 && m_SecondAttackFrame <= 0)
		{
			if (m_isNextOnAttack)
			{
				m_TransAnim->SetAnimationFrame(0);
				m_isNextOnAttack = false;
				m_isSecondAttack = true;
				m_isAttack = false;
				m_AttackFrame = 0;
				m_TransAnim->SetAnimationState("Attack2");
				if (!m_IsBuff) {
					m_SecondAttackFrame = 68;
				}
				else {
					m_SecondAttackFrame = 34;
				}
			}
			else {
				m_isSecondAttack = false;
			}
		}

		//if (m_isAttackCancel)
		//{
		//	GetComponent<Transform3DAnimationComponent>()->SetAnimationFrame(0);
		//	m_isAttack = false;
		//	m_isSecondAttack = false;
		//	m_isNextOnAttack = false;
		//}

		//捕食攻撃の処理
		if (m_isPredation) m_PredationFrame--;
		if (!m_isPredation && Input::GetKeyTrigger('R'))
		{
			m_TransAnim->SetAnimationFrame(0);
			m_isPredation = true;
			m_TransAnim->SetAnimationState("PredationAttack");
			if (!m_IsBuff) {
				m_PredationFrame = 50;
			}
			else {
				m_PredationFrame = 25;
			}
		}

		if (m_PredationFrame == 20) PredationAttack();

		if (m_PredationFrame <= 0) {
			m_isPredation = false;
			m_PredationFrame = 0;
		}
	}
	else	//銃モード
	{
		if (Input::GetKeyTrigger('F') || Input::GetKeyTrigger(MOUSEEVENTF_MOVE))
		{
			if (m_BulletPoint > 0.0f)
			{
				XMFLOAT3 pos = m_TransAnim->GetPos();
				XMFLOAT3 rot = m_TransAnim->GetRot();
				m_dir.x = cosf(rot.y - (XM_PI / 2));
				m_dir.y = 0.0f;
				m_dir.z = sinf(rot.y + (XM_PI / 2));
				Bullet* bullet = Scene::GetInstance()->GetScene<Game>()->AddGameObject<Bullet>(1);
				bullet->GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(pos.x, pos.y + 1.0f, pos.z));
				m_SE->Play();
				m_BulletPoint -= 10.0f;
			}
		}
	}
}

//死亡時のアニメーション
void Player::DeathAnim()
{
	ResetFlag();
	if (m_IsDethAnim) m_DyingFrame--;
	if (!m_IsDethAnim && !m_IsDie)
	{
		m_TransAnim->SetAnimationFrame(0);
		m_TransAnim->SetAnimationState("Dying");
		m_IsDethAnim = true;
		m_DyingFrame = 104;
	}

	if (m_DyingFrame <= 0 && !m_IsDie)
	{
		m_IsDie = true;
		m_IsDethAnim = false;
		m_DyingFrame = 0;
		m_TransAnim->SetAddAnimFrame(0);
		XMFLOAT3 pos = m_TransAnim->GetPos();
	}

	if (m_IsDie) m_DieFrame++;

	if (m_DieFrame >= 120) {
		m_shoudNextScene = true;
	}

}

//デバッグ用ImGui
void Player::DrawImGui()
{
	XMFLOAT3 transPos = m_TransAnim->GetPos();
	XMFLOAT3 transRot = m_TransAnim->GetRot();
	XMFLOAT3 coliderPos = GetColider()->GetPos();
	XMFLOAT3 coliderRot = GetColider()->GetRot();

	{
		ImGui::Begin("Player");

		ImGui::Text("This is some useful text.");
		ImGui::Text("PlayerPos : x = %.1f, y = %.1f, z = %.1f", transPos.x, transPos.y, transPos.z);
		ImGui::Text("PlayerRot : x = %.1f, y = %.1f, z = %.1f", transRot.x, transRot.y, transRot.z);
		ImGui::Text("ColiderPos : x = %.1f, y = %.1f, z = %.1f", coliderPos.x, coliderPos.y, coliderPos.z);
		ImGui::Text("IsGravity : %d", m_IsGravity);

		ImGui::End();
	}

}
