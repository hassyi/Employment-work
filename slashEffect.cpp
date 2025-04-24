#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "slashEffect.h"
#include "transform2DComponent.h"
#include "transform3DComponent.h"
#include "game.h"
#include "player.h"

void SlashEffect::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-2.0f, 2.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(2.0f, 2.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-2.0f, -2.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(2.0f, -2.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	Player* player = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Player>();

	if (player->GetIsAttack()) {
		m_TextureData = L"asset\\texture\\slashEffect1.png";
	}
	else if (player->GetIsSecondAttack()) {
		m_TextureData = L"asset\\texture\\slashEffect2.png";
	}

	AddComponent<Transform2DComponent>()->AddTextureData(m_TextureData);
	GetComponent<Transform2DComponent>()->SetVertex(vertex);
	GetComponent<Transform2DComponent>()->SetBillBoard(true);

	for (auto component : m_ComponentList)
	{
		component->Init();
	}

	m_IsDraw = true;
}

void SlashEffect::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void SlashEffect::Update()
{
	++m_Count;

	if (m_Count >= 9) {
		m_IsDraw = false;
		return;
	}

}

void SlashEffect::Draw()
{
	if (m_IsDraw)
	{
		//テクスチャ座標算出
		float x = m_Count % 9 * (1.0f / 9);
		float y = m_Count / 9 * (1.0f / 1);

		VERTEX_3D vertex[4];

		vertex[0].Position = XMFLOAT3(-2.0f, 2.0f, 0.0f);
		vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = XMFLOAT2(x, y);

		vertex[1].Position = XMFLOAT3(2.0f, 2.0f, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = XMFLOAT2(x + 0.11f, y);

		vertex[2].Position = XMFLOAT3(-2.0f, -2.0f, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = XMFLOAT2(x, y + 1.0f);

		vertex[3].Position = XMFLOAT3(2.0f, -2.0f, 0.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = XMFLOAT2(x + 0.11f, y + 1.0f);

		for (auto component : m_ComponentList)
		{
			GetComponent<Transform2DComponent>()->SetVertex(vertex);
			component->Draw();
		}
	}
}
