#include "player.h"
#include "animationModel.h"
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
#include "transform2DComponent.h"
#include "transform3DAnimaitonComponent.h"
#include "game.h"
#include "boxColiderComponent.h"
#include "buffParticle.h"

void Player::Init()
{
	m_Component = new AnimationModel(this);
	((AnimationModel*)m_Component)->Load("asset\\model\\Vampire A Lusth.fbx");
	((AnimationModel*)m_Component)->LoadAnimation("asset\\model\\Idle.fbx", "Idle");
	((AnimationModel*)m_Component)->LoadAnimation("asset\\model\\Running.fbx", "Run");

	//AddComponent<Transform3DAnimationComponent>()->AddModelData("asset\\model\\Vampire A Lusth.fbx", Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>());
	//GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\Idle.fbx", "Idle");
	//GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\model\\Running.fbx", "Run");
	AddComponent<Transform3DComponent>();
	AddComponent<BoxColiderComponent>();
	GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(0.01f, 0.01f, 0.01f));
	GetComponent<Colider>()->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	SetScale(GetComponent<Transform3DComponent>()->GetScale());

	//for (auto component : m_ComponentList)
	//{
	//	component->Init();
	//}

	m_AnimationName1 = "Idle";
	m_AnimationName2 = "Idle";

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_ObjType = OBJ_TYPE::PLAYER;
	//m_SE = new Audio(this);
	//m_SE->Load("asset\\audio\\shot.wav");

	//m_ChildModel = new ModelRenderer(this);
	//((ModelRenderer*)m_ChildModel)->Load("asset\\model\\box.obj");



}

void Player::Uninit()
{
	//m_SE->Uninit();
	//delete m_SE;

	m_Component->Uninit();
	delete m_Component;

	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	m_Predation = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Predation>();

	m_AnimationName = "Idle";
	
	PlayerControl();

	SetPos(GetComponent<Transform3DComponent>()->GetPos());
	SetRot(GetComponent<Transform3DComponent>()->GetRot());
	SetVel(GetComponent<Transform3DComponent>()->GetVel());
	GetComponent<Transform3DComponent>()->SetOldPos(GetComponent<Transform3DComponent>()->GetPos());
	SetOldPos(GetComponent<Transform3DComponent>()->GetOldPos());


	PlayerCollision();

	for (auto component : m_ComponentList)
	{
		component->Update();
	}

	if (Input::GetKeyTrigger('F')) 
	{
		m_dir.x = cosf(GetRot().y - (XM_PI / 2));
		m_dir.y = 0.0f;
		m_dir.z = sinf(GetRot().y + (XM_PI / 2));
		Bullet* bullet = Scene::GetInstance()->GetScene<Game>()->AddGameObject<Bullet>(1);
		bullet->GetComponent<Transform3DComponent>()->SetPos(GetPos());
		//m_SE->Play();
	}

	PredationAttack();
	PlayerBuff();

	GetComponent<Transform3DComponent>()->SetOldPos(GetComponent<Transform3DComponent>()->GetPos());

	if (Input::GetKeyTrigger(VK_RETURN)) {
		Scene::GetInstance()->ChangeScene(new Result);
	}

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
	//for (auto component : m_ComponentList)
	//{
	//	if (component == (AnimationModel*)component)
	//	{
	//		((AnimationModel*)component)->UpdateAnimation(m_AnimationName1.c_str(), m_AnimationFrame, m_AnimationName2.c_str(), m_AnimationFrame, m_AnimetionBlendRatio);
	//	}
	//	component->Draw();
	//}

	((AnimationModel*)m_Component)->UpdateAnimation(m_AnimationName1.c_str(), m_AnimationFrame,
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
	Predation* predation = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Predation>();

	//XMFLOAT3 pos = GetComponent<Transform3DAnimationComponent>()->GetPos();
	//XMFLOAT3 oldPos = GetComponent<Transform3DAnimationComponent>()->GetOldPos();
	//XMFLOAT3 vel = GetComponent<Transform3DAnimationComponent>()->GetVel();
	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 oldPos = GetComponent<Transform3DComponent>()->GetOldPos();
	XMFLOAT3 vel = GetComponent<Transform3DComponent>()->GetVel();

	float dt = 30.0f * (1.0f / 60.0f);
	float dt1 = 1.0f / 60.0f;

	if (Input::GetKeyPress(VK_LSHIFT) || predation->GetBuff()) {
		m_move = 0.5f;
	}
	else {
		m_move = 0.2f;
	}

	if (Input::GetKeyPress('W'))
	{
		vel.x += m_move * sinf(camera->GetRot().y);
		vel.z += m_move * cosf(camera->GetRot().y);
		m_Rot = 0.0f;
		m_AnimationName = "Run";
	}
	if (Input::GetKeyPress('A'))
	{
		vel.x -= m_move * cosf(camera->GetRot().y);
		vel.z += m_move * sinf(camera->GetRot().y);
		m_Rot = -3.14f / 2.0f;
		m_AnimationName = "Run";
	}
	if (Input::GetKeyPress('S'))
	{
		vel.x -= m_move * sinf(camera->GetRot().y);
		vel.z -= m_move * cosf(camera->GetRot().y);
		m_Rot = 3.14f;
		m_AnimationName = "Run";
	}
	if (Input::GetKeyPress('D'))
	{
		vel.x += m_move * cosf(camera->GetRot().y);
		vel.z -= m_move * sinf(camera->GetRot().y);
		m_Rot = 3.14f / 2.0f;
		m_AnimationName = "Run";
	}
		//ジャンプ
	if (Input::GetKeyPress(VK_SPACE))
	{
		if (!m_JampFlag)
		{
			vel.y = 20.0f;
			m_JampFlag = true;
		}
	}


	if (m_IsGravity) {
		m_Gravity = -30.0f;
	}
	if (!m_IsGravity)
	{
		pos.y = oldPos.y;
		vel.y = 0.0f;
		m_Gravity = 0.0f;
		m_JampFlag = false;
	}

	vel.x *= dt;
	vel.z *= dt;
	vel.y += m_Gravity * dt1;

	if (pos.y < m_GroundHeight)
	{
		pos.y = m_GroundHeight;
		vel.y = 0.0f;
		m_JampFlag = false;
		m_IsGravity = false;
	}
	else {
		m_IsGravity = true;
	}

	GetComponent<Transform3DComponent>()->SetVel(vel);
	GetComponent<Transform3DComponent>()->SetRotY(m_Rot + camera->GetRot().y);
	GetComponent<Transform3DComponent>()->SetPosX(pos.x + vel.x * dt);
	GetComponent<Transform3DComponent>()->SetPosZ(pos.z + vel.z * dt);
	GetComponent<Transform3DComponent>()->SetPosY(pos.y + vel.y * dt1);
	GetColider()->SetPos(pos);
	GetColider()->SetOldPos(oldPos);
}

void Player::PlayerCollision()
{
	//メッシュフィールド高さ取得
	MeshField* meshField = Scene::GetInstance()->GetScene<Game>()->GetGameObject<MeshField>();

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 oldPos = GetComponent<Transform3DComponent>()->GetOldPos();
	XMFLOAT3 scale = GetComponent<Transform3DComponent>()->GetScale();
	XMFLOAT3 vel = GetComponent<Transform3DComponent>()->GetVel();

	//地面の高さ
	m_GroundHeight = meshField->GetHeight(pos);

	//auto cylinderList = Scene::GetInstance()->GetScene<Game>()->GetGameObjects<Cylinder>();

	//for (auto cylinder : cylinderList) {
	//	XMFLOAT3 cylinderPos = cylinder->GetPos();
	//	XMFLOAT3 cylinderScale = cylinder->GetScale();

	//	if (collision.CollisionCylinder(pos, cylinderPos, cylinderScale.x))
	//	{
	//		if (collision.CollisionCylinderHeight(pos, cylinderPos, cylinderScale.y)) {
	//			m_GroundHeight = cylinderPos.y + cylinderScale.y;
	//		}
	//		else 
	//		{
	//			pos.x = oldPos.x;
	//			pos.z = oldPos.z;
	//			vel.x = 0.0f;
	//			vel.z = 0.0f;
	//		}
	//	}
	//}
	
	if (std::get<0>(GetColider()->GetCollision()) == true)
	{
		std::list<GameObject*> objectList = std::get<2>(GetComponent<Colider>()->GetCollision());
		for (auto onCollisionObject : objectList)
		{
			if (onCollisionObject->GetObjectType() == OBJ_TYPE::BOX)
			{
				float boxposy = onCollisionObject->GetColider()->GetPos().y + (onCollisionObject->GetColider()->GetScale().y * 2) + 1.0f;
				if (pos.y - scale.y >= boxposy)
				{
					m_GroundHeight = boxposy + onCollisionObject->GetColider()->GetScale().y;
					m_Gravity = 0.0f;
					m_JampFlag = false;
				}
				else
				{
					float geta = 1.0f;
					float boxPosX = onCollisionObject->GetColider()->GetPos().x;
					float boxPosZ = onCollisionObject->GetColider()->GetPos().z;
					float boxScaleX = onCollisionObject->GetColider()->GetScale().x;
					float boxScaleZ = onCollisionObject->GetColider()->GetScale().z;
					float disx = pos.x - boxPosX;
					float disz = pos.z - boxPosZ;

					//計算しやすいように距離の値を＋にする
					if (disx < 0) {
						disx *= -1;
					}
					if (disz < 0) {
						disz *= -1;
					}

					//当たったオブジェクトの角度を調べる
					float rotbb = atan(disx / disz);

					//当たったオブジェクトの中心からの角度
					float rot = atan(boxScaleX / boxScaleZ);

					if (rotbb >= rot)
					{
						if (0.0f > pos.x - boxPosX) {
							pos.x = (boxPosX - boxScaleX - geta) - scale.x / 2;
						}
						else if (0.0f < pos.x - boxPosX) {
							pos.x = (boxPosX + boxScaleX + geta) + scale.x / 2;
						}
					}
					else if (rotbb <= rot)
					{
						if (0.0f > pos.z - boxPosZ)	{
							pos.z = (boxPosZ - boxScaleZ - geta) - scale.z / 2;
						}
						else if (0.0f < pos.z - boxPosZ) {
							pos.z = (boxPosZ + boxScaleZ + geta) + scale.z / 2;
						}
					}
					GetComponent<Transform3DComponent>()->SetPos(pos);
					GetColider()->SetPos(pos);
				}
			}
			else if (onCollisionObject->GetObjectType() == OBJ_TYPE::CYLINDER)
			{
				XMFLOAT3 cylinderPos = onCollisionObject->GetColider()->GetPos();
				XMFLOAT3 cylinderScale = onCollisionObject->GetColider()->GetScale();
				float directionY = pos.y - cylinderPos.y;
				if (directionY > cylinderScale.y - 0.5f)
				{
					m_GroundHeight = cylinderPos.y + cylinderScale.y;
					m_Gravity = 0.0f;
					m_JampFlag = false;
				}
				else
				{
					pos.x = (cylinderPos.x + cylinderScale.x) + scale.x / 2;
					pos.z = (cylinderPos.z + cylinderScale.z) + scale.z / 2;
					vel.x = 0.0f;
					vel.z = 0.0f;
				}
			}
		}
	}
	else
	{
		GetComponent<Transform3DComponent>()->SetPos(pos);
		GetComponent<Transform3DComponent>()->SetOldPos(oldPos);
		GetComponent<Transform3DComponent>()->SetVel(vel);
		GetColider()->SetPos(pos);
	}

}

void Player::PredationAttack()
{
	Predation* predation = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Predation>();

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 scale = GetComponent<Transform3DComponent>()->GetScale();
	XMFLOAT3 rot = GetComponent<Transform3DComponent>()->GetRot();

	XMFLOAT3 predationPos;
	predationPos.x = pos.x + 1.0f * cosf(rot.y - (XM_PI / 2));
	predationPos.z = pos.z + 1.0f * sinf(rot.y + (XM_PI / 2));
	predationPos.y = pos.y;

	if (Input::GetKeyTrigger('R')) 
	{
		predation->SetUse(true);
		predation->GetComponent<Transform3DComponent>()->SetPos(predationPos);
		GetComponent<Transform3DComponent>()->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}

}

void Player::PlayerBuff()
{
	m_BuffParticle = Scene::GetInstance()->GetScene<Game>()->GetGameObject<BuffParticle>();

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPos();

	m_BuffParticle->SetPlayerBuff(m_Predation->GetBuff());
	m_BuffParticle->GetComponent<Transform2DComponent>()->SetPos(pos);
}
