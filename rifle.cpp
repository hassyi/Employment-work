#include "rifle.h"
#include "scene.h"
#include "player.h"
#include "transform3DComponent.h"
#include "transform3DAnimaitonComponent.h"
#include "game.h"

void Rifle::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\model\\rifle.obj");
	GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
}

void Rifle::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void Rifle::Update()
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	if (player->GetWeaponType() == WEAPON_TYPE::GUN)
	{
		Transform3DAnimationComponent* playerTrans = player->GetComponent<Transform3DAnimationComponent>();

		XMMATRIX rightHand = playerTrans->GetModel()->GetRightHandMatrix();
		Transform3DComponent* trans = GetComponent<Transform3DComponent>();
		trans->SetParent(player);

		trans->SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));

		trans->SetPos(XMFLOAT3(10.0f, 35.0f, 0.0f));
		trans->SetLocalMatrix(rightHand);


		for (auto component : m_ComponentList)
		{
			component->Update();
		}
	}
}

void Rifle::Draw()
{
	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();
	if (player->GetWeaponType() == WEAPON_TYPE::GUN)
	{
		for (auto component : m_ComponentList)
		{
			component->Draw();
		}

		if (Scene::GetInstance()->GetScene<Game>()->GetIsDrawImGui()) {
			DrawImGui();
		}
	}
}

void Rifle::DrawImGui()
{
	XMFLOAT3 transPos = GetComponent<Transform3DComponent>()->GetPos();
	XMFLOAT3 transRot = GetComponent<Transform3DComponent>()->GetRot();

	{
		ImGui::Begin("Rifle");

		ImGui::Text("RiflePos : x = %.1f, y = %.1f, z = %.1f", transPos.x, transPos.y, transPos.z);
		ImGui::Text("RifleRot : x = %.1f, y = %.1f, z = %.1f", transRot.x, transRot.y, transRot.z);

		ImGui::End();
	}
}
