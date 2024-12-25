#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "fireParticle.h"
#include "game.h"
#include "transform2DComponent.h"
#include "bonFire.h"
#include "title.h"


void FireParticle::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 0.4f, 0.1f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 0.4f, 0.1f, 1.0f);;
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 0.4f, 0.1f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 0.4f, 0.1f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	AddComponent<Transform2DComponent>()->AddTextureData(L"asset\\texture\\particle.png");
	GetComponent<Transform2DComponent>()->SetVertex(vertex);
	GetComponent<Transform2DComponent>()->SetBillBoard(true);
	GetComponent<Transform2DComponent>()->SetZBuff(true);
	GetComponent<Transform2DComponent>()->SetIsTitle(true);

	GetComponent<Transform2DComponent>()->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));

	for (auto component : m_ComponentList)
	{
		component->Init();
	}
}

void FireParticle::Uninit()
{
	for (auto component : m_ComponentList)
	{
		component->Uninit();
		delete component;
	}
}

void FireParticle::Update()
{
	//パーティクル発射
	for (int i = 0; i < PARTICLE_MAX; i++) {
		if (!m_Particle[i].Enable) {
			m_Particle[i].Enable = true;

			m_Particle[i].Pos = XMFLOAT3(2.0f, 0.0f, 0.0f);
			m_Particle[i].Pos.y = (rand() % 100 + 50) / 250.0f;
			m_Particle[i].Vel.y = (rand() % 100 + 50) / 1000.0f;

			m_Particle[i].Life = 20;

			break;
		}
	}
	for (int i = 0; i < PARTICLE_MAX; i++) {
		if (m_Particle[i].Enable) {
			m_Particle[i].Pos.y += m_Particle[i].Vel.y;
			m_Particle[i].Pos.x += (rand() % 100 - 50) / 1000.0f;
			m_Particle[i].Pos.z += (rand() % 100 - 50) / 1000.0f;

			m_Particle[i].Life--;
			if (m_Particle[i].Life < 0) {
				m_Particle[i].Enable = false;
			}
		}

	}
}

void FireParticle::Draw()
{
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable) 
		{
			for (auto component : m_ComponentList)
			{
				GetComponent<Transform2DComponent>()->SetPos(m_Particle[i].Pos);
				component->Draw();
			}
		}
	}

}
