#include "aimIcon.h"
#include "textureComponent.h"
#include "texture2D.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "transform3DAnimaitonComponent.h"

void AimIcon::Init()
{
	m_Pos = { 620.0f, 210.0f };
	m_Size = { 100.0f, 100.0f };

	AddComponent<TextureComponent>()->SetTransTexNum(m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, L"asset\\texture\\AimIcon.png", 1);
	GetComponent<TextureComponent>()->SetDisitDisplay(false);
	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void AimIcon::Uninit()
{
	for (auto component : m_ComponentList)
	{
		GetComponent<TextureComponent>()->Uninit();
		delete component;
	}
}

void AimIcon::Update()
{
	if (m_IsDraw)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		Renderer::GetSwapChain()->GetDesc(&swapChainDesc);

		int screenWidth = swapChainDesc.BufferDesc.Width;
		int screenHeight = swapChainDesc.BufferDesc.Height;

		m_CenterPosition = { screenWidth * 0.5f, screenHeight * 0.5f };


		for (auto object : Scene::GetInstance()->GetScene<Game>()->GetGameObjectList())
		{
			if (object->GetObjectType() == OBJ_TYPE::ENEMY)
			{
				float posx, posy;
				XMFLOAT3 enemyPos = object->GetComponent<Transform3DAnimationComponent>()->GetPos();

				if (ObjectInCircle(enemyPos, screenWidth, screenHeight, posx, posy))
				{
					GetComponent<TextureComponent>()->SetDiffuse(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
				}
				else
				{
					GetComponent<TextureComponent>()->SetDiffuse(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
		}
	}

}

void AimIcon::Draw()
{
	if (m_IsDraw)
	{
		GetComponent<TextureComponent>()->SetPosSize(m_CenterPosition.x - m_Radius, m_CenterPosition.y - 70.0f - m_Radius, m_Radius * 2, m_Radius * 2);
		GetComponent<TextureComponent>()->SetAnimation(0.0f, 0.0f, 1.0f, 1.0f);
		for (auto component : m_ComponentList)
		{
			component->Draw();
		}
	}
}

//エイムアイコン内にエネミーが入ったかの判定
bool AimIcon::ObjectInCircle(XMFLOAT3 worldPos, int screenWidth, int screenHeight, float& screenPosX, float& screenPosY)
{
	worldPos.y += 0.5f;
	XMVECTOR worldPosition = XMLoadFloat3(&worldPos);
	XMVECTOR viewPosition = XMVector3TransformCoord(worldPosition, m_ViewMatrix);
	XMVECTOR clipPosition = XMVector3TransformCoord(viewPosition, m_ProjectionMatrix);


	XMFLOAT4 clipPos;
	XMStoreFloat4(&clipPos, viewPosition);

	XMFLOAT4 ndc;
	ndc = clipPos;

	if (clipPos.z < 0.0f)
	{
		return false;
	}


	XMFLOAT3 ndcPos;
	XMStoreFloat3(&ndcPos, clipPosition);

	screenPosX = (ndcPos.x + 1.0f) * 0.5f * screenWidth;
	screenPosY = (1.0f - ndcPos.y) * 0.5f * screenHeight;

	float distance = sqrtf(powf(screenPosX - m_CenterPosition.x, 2) + powf(screenPosY - m_CenterPosition.y, 2));
	return (distance <= m_Radius);

}
