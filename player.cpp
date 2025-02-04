#include "player.h"
#include "input.h"
#include "camera.h"
#include "scene.h"
#include "manager.h"
#include "bullet.h"
#include "title.h"
#include "result.h"
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


void Player::Init()
{
	SCENE_STATE scene = Scene::GetInstance()->GetNowScene()->GetSceneState();

	AddComponent<Transform3DAnimationComponent>()->AddModelData("asset\\model\\Vampire A Lusth.fbx", this);
	if (scene == SCENE_STATE::SCENE_TITLE) {
		GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\Idle_Title.fbx", "TitleIdle");
		GetComponent<Transform3DAnimationComponent>()->SetInitAnimationState("TitleIdle");
		return;
	}
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\Idle.fbx", "Idle");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\Running.fbx", "Run");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\Attack1.fbx", "Attack1");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\Attack2.fbx", "Attack2");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\predationAttack.fbx", "PredationAttack");
	GetComponent<Transform3DAnimationComponent>()->SetInitAnimationState("Idle");

	AddComponent<Transform>();
	GetComponent<Transform>()->SetScale(XMFLOAT3(0.01f, 0.01f, 0.01f));
	//AddComponent<BoxColiderComponent>();
	AddComponent<CapsuleColiderComponent>();
	AddComponent<SphereShadow>()->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));

	for (auto component : m_ComponentList)
	{
		component->Init();
	}

	m_Life = 10.0f;
	m_BulletPoint = 100.0f;

	//Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
	//	"shader\\unlitTextureVS.cso");

	//Renderer::CreatePixelShader(&m_PixelShader,
	//	"shader\\unlitTexturePS.cso");

	m_ObjType = OBJ_TYPE::PLAYER;
	//m_SE = new Audio(this);
	//m_SE->Load("asset\\audio\\shot.wav");

	//m_ChildModel = new ModelRenderer(this);
	//((ModelRenderer*)m_ChildModel)->Load("asset\\model\\box.obj");

	GetComponent<CapsuleColiderComponent>()->SetScale(XMFLOAT3(0.5f, 1.0f, 0.5f));
	GetComponent<CapsuleColiderComponent>()->SetSegmentLength(1.0f);


}

void Player::Uninit()
{
	//m_SE->Uninit();
	//delete m_SE;

	//m_Component->Uninit();
	//delete m_Component;

	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}

	//m_VertexLayout->Release();
	//m_VertexShader->Release();
	//m_PixelShader->Release();
}

void Player::Update()
{
	SCENE_STATE scene = Scene::GetInstance()->GetNowScene()->GetSceneState();
	if (scene == SCENE_STATE::SCENE_TITLE)
	{
		GetComponent<Transform3DAnimationComponent>()->SetAnimationState("TitleIdle");
		return;
	}
	
	m_Predation = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Predation>();

	PlayerControl();
	PlayerCollision();

	GetComponent<SphereShadow>()->SetPos(GetComponent<Transform>()->GetPos());

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
		GetComponent<Transform3DAnimationComponent>()->SetAnimationState("TitleIdle");
		return;
	}

	DrawImGui();

	//子モデル描画
	//std::unordered_map<std::string, BONE> bone;
	//bone = ((AnimationModel*)m_Component)->GetBone();
	//bone.find("mixamorig:RightHand");
	//
	//XMMATRIX childScale, childRot, childTrans, childWorld, boneMatrix;
	//rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	//childScale = XMMatrixScaling(1.0f / GetScale().x, 1.0f / GetScale().y, 1.0f / GetScale().z);
	//childTrans = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	//childWorld = childTrans * childScale * world;
	//子のワールド = 子のローカル * 親のワールド
	//childWorld = childTrans * childScale  * 指定した部位のマトリクス * world;

	//Renderer::SetWorldMatrix(childWorld);

	//m_ChildModel->Draw();
}

void Player::PlayerControl()
{
	Camera* camera = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Camera>();

	XMFLOAT3 pos = GetComponent<Transform>()->GetPos();
	XMFLOAT3 vel = GetComponent<Transform>()->GetVel();
	XMFLOAT3 rot = GetComponent<Transform>()->GetRot();

	float dt = 30.0f * (1.0f / 60.0f);
	float dt1 = 1.0f / 60.0f;

	if (Input::GetKeyPress(VK_LSHIFT) || m_IsBuff) {
		PlayerBuff();
		m_move = 0.6f;
		m_StepSpeed = 5.0f;
	}
	else {
		m_move = 0.3f;
		m_StepSpeed = 3.0f;
	}

	if (Input::GetKeyPress('W')){
		m_Rot = 0.0f;
	}
	if (Input::GetKeyPress('A')){
		m_Rot = -XM_PI / 2.0f;
	}
	if (Input::GetKeyPress('S')){
		m_Rot = XM_PI;
	}
	if (Input::GetKeyPress('D')){
		m_Rot = XM_PI / 2.0f;
	}

	if (m_AttackFrame <= 0 && m_SecondAttackFrame <= 0 && m_PredationFrame <= 0)
	{
		if (Input::GetKeyPress('W') || Input::GetKeyPress('A') || Input::GetKeyPress('S') || Input::GetKeyPress('D'))
		{
			rot = XMFLOAT3(rot.x, m_Rot + camera->GetRot().y, rot.z);
			vel = XMFLOAT3(sinf(rot.y) * m_move, vel.y, cosf(rot.y) * m_move);
			GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Run");
		}
		else {
			GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");
		}
	}



	//ステップ
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

	//武器切り替え
	if (Input::GetKeyTrigger('C'))
	{
		if (m_Weapon == SWORD){
			m_Weapon = GUN;
		}
		else {
			m_Weapon = SWORD;
		}
	}

	PlayerAttack();

	if (m_IsGravity) {
		m_Gravity -= 0.5f;
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
	if (Input::GetKeyPress(VK_SPACE))
	{
		if (!m_JampFlag)
		{
			vel.y = 10.0f;
			m_JampFlag = true;
		}
	}

	//if (pos.y < m_GroundHeight)
	//{
	//	pos.y = m_GroundHeight;
	//	vel.y = 0.0f;
	//	m_JampFlag = false;
	//	m_IsGravity = false;
	//}


	GetComponent<Transform>()->SetVel(vel);
	GetComponent<Transform>()->SetRot(rot);
	GetComponent<Transform>()->SetPosX(pos.x + vel.x * dt);
	GetComponent<Transform>()->SetPosZ(pos.z + vel.z * dt);
	GetComponent<Transform>()->SetPosY(pos.y + vel.y * dt1);
	GetColider()->SetPos(GetComponent<Transform>()->GetPos());
}

void Player::PlayerCollision()
{
	Transform* transform = GetComponent<Transform>();
	Colider* colider = GetComponent<Colider>();

	XMFLOAT3 pos = transform->GetPos();
	XMFLOAT3 scale = transform->GetScale();
	XMFLOAT3 vel = transform->GetVel();
	XMFLOAT3 coliderPos = colider-> GetPos();
	XMFLOAT3 coliderScale = colider->GetScale();
	//地面の高さ
	m_GroundHeight = Scene::GetInstance()->GetScene<Game>()->GetGameObject<MeshField>()->GetHeight(pos);
	
	std::tuple<bool, GameObject*, std::list<GameObject*>> objectList = colider->GetCollision();

	if (std::get<0>(objectList))
	{
		m_IsGravity = true;

		if (!m_JampFlag)
		{
			if (pos.y <= m_GroundHeight + 0.1f)
			{
				pos.y = m_GroundHeight;
				m_IsGravity = false;
			}
		}
	}
	else
	{
		m_IsGravity = true;
		if (!m_JampFlag)
		{
			if (pos.y <= m_GroundHeight + 0.1f)
			{
				pos.y = m_GroundHeight;
				m_IsGravity = false;
			}
		}
		else
		{
			m_IsGravity = false;
		}
		transform->SetPos(pos);
		colider->SetPos(pos);

	}


	//if (GetComponent<Colider>()->GetColiderType()==COLIDER_TYPE::CAPSULE_COLIDER)
	//{
	//	std::tuple<bool, GameObject*, std::list<GameObject*>> objectList = GetComponent<CapsuleColiderComponent>()->GetCollision();
	//	
	//	if(std::get<0>(objectList))
	//	{
	//		std::list<GameObject*> objects = std::get<2>(objectList);
	//		for (auto onCollisionObject : objects)
	//		{
	//			//if (onCollisionObject->GetObjectType() == OBJ_TYPE::BOX)
	//			//{
	//				float boxposy = onCollisionObject->GetColider()->GetPos().y;
	//				if (coliderPos.y - coliderScale.y > boxposy)
	//				{
	//					m_GroundHeight = boxposy + onCollisionObject->GetColider()->GetScale().y;
	//					m_Gravity = 0.0f;
	//					m_JampFlag = false;
	//				}
	//				GetComponent<Transform>()->SetPos(coliderPos);
	//				GetColider()->SetPos(coliderPos);
	//				GetComponent<Transform>()->SetVel(vel);
	//			//}
	//		}
	//	}
	//}
	//else
	//{
	//	GetComponent<Transform>()->SetPos(pos);
	//	GetComponent<Transform>()->SetVel(vel);
	//	GetColider()->SetPos(pos);
	//}

}

void Player::PredationAttack()
{
	XMFLOAT3 pos = GetComponent<Transform>()->GetPos();
	XMFLOAT3 scale = GetComponent<Transform>()->GetScale();
	XMFLOAT3 rot = GetComponent<Transform>()->GetRot();

	XMFLOAT3 predationPos;
	predationPos.x = pos.x + 2.0f * cosf(rot.y - (XM_PI / 2));
	predationPos.z = pos.z + 2.0f * sinf(rot.y + (XM_PI / 2));
	predationPos.y = pos.y;

	m_Predation = Scene::GetInstance()->GetScene<Game>()->AddGameObject<Predation>(1);
	m_Predation->GetComponent<Transform3DComponent>()->SetPos(predationPos);
	GetComponent<Transform>()->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
}

void Player::PlayerBuff()
{
	m_BuffParticle = Scene::GetInstance()->GetScene<Game>()->GetGameObject<BuffParticle>();
	if (m_BuffParticle == nullptr) return;

	XMFLOAT3 pos = GetComponent<Transform>()->GetPos();

	m_BuffParticle->SetPlayerBuff(m_IsBuff);
	m_BuffParticle->GetComponent<Transform2DComponent>()->SetPos(pos);
}

void Player::PlayerAttack()
{
	Enemy* enemy = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Enemy>();

	if (m_Weapon == SWORD)
	{
		if (m_isAttack) m_AttackFrame--;
		if (!m_isAttack && !m_isSecondAttack)
		{
			if (Input::GetKeyTrigger('F') || Input::GetKeyTrigger(MOUSEEVENTF_MOVE))		//MOUSEEVENTF_MOVE←これがなぜか左クリック
			{
				GetComponent<Transform3DAnimationComponent>()->SetAnimationFrame(0);
				m_isAttack = true;
				m_AttackFrame = 72;

				GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Attack1");
			}
		}
		//ここで当たり判定をつける
		if (m_AttackFrame == 25) {
			enemy->SetLife(enemy->GetLife() - 1);
		}
		if (m_AttackFrame == 10)
		{
			m_isNext = true;
			m_AttackReseveFrame = 30;
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

		if (m_isSecondAttack) m_SecondAttackFrame--;
		if (m_SecondAttackFrame == 30) {
			enemy->SetLife(enemy->GetLife() - 1);
		}

		if (m_AttackFrame <= 0 && m_SecondAttackFrame <= 0)
		{
			if (m_isNextOnAttack)
			{
				GetComponent<Transform3DAnimationComponent>()->SetAnimationFrame(0);
				m_isNextOnAttack = false;
				m_isSecondAttack = true;
				m_SecondAttackFrame = 68;
				m_isAttack = false;
				m_AttackFrame = 0;
				GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Attack2");
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


		if (m_isPredation) m_PredationFrame--;
		if (!m_isPredation && Input::GetKeyTrigger('R'))
		{
			GetComponent<Transform3DAnimationComponent>()->SetAnimationFrame(0);
			m_isPredation = true;
			m_PredationFrame = 50;
			GetComponent<Transform3DAnimationComponent>()->SetAnimationState("PredationAttack");
		}

		if (m_PredationFrame == 20) PredationAttack();

		if (m_PredationFrame <= 0) {
			m_isPredation = false;
			m_PredationFrame = 0;
		}
	}
	else
	{
		if (Input::GetKeyTrigger('F') || Input::GetKeyTrigger(MOUSEEVENTF_MOVE))
		{
			if (m_BulletPoint > 0.0f)
			{
				m_dir.x = cosf(GetComponent<Transform>()->GetRot().y - (XM_PI / 2));
				m_dir.y = 0.0f;
				m_dir.z = sinf(GetComponent<Transform>()->GetRot().y + (XM_PI / 2));
				Bullet* bullet = Scene::GetInstance()->GetScene<Game>()->AddGameObject<Bullet>(1);
				bullet->GetComponent<Transform3DComponent>()->SetPos(GetComponent<Transform>()->GetPos());
				//m_SE->Play();
				m_BulletPoint -= 10.0f;
			}
		}
	}
}

void Player::DrawImGui()
{
	XMFLOAT3 transPos = GetComponent<Transform>()->GetPos();
	XMFLOAT3 transRot = GetComponent<Transform>()->GetRot();
	XMFLOAT3 coliderPos = GetColider()->GetPos();
	XMFLOAT3 coliderRot = GetColider()->GetRot();

	{
		ImGui::Begin("Player");

		ImGui::Text("This is some useful text.");
		ImGui::Text("PlayerPos : x = %.1f, y = %.1f, z = %.1f", transPos.x, transPos.y, transPos.z);
		//ImGui::Text("IsGravity : %d", m_IsGravity);

		ImGui::End();
	}

}
