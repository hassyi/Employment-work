#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "sandParticle.h"
#include "game.h"
#include "transform2DComponent.h"
#include "titleCamera.h"
#include "title.h"

void SandParticle::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-m_Size, m_Size, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(m_Size, m_Size, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-m_Size, -m_Size, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(m_Size, -m_Size, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		m_Particle[i].Vel = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_Particle[i].Life = 300;
		m_Particle[i].Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);



	//シェーダーリソースビュー
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;
	Renderer::GetDevice()->CreateShaderResourceView(m_ParticleBuffer, &srvd, &m_ParticleSRV);
	Renderer::GetDevice()->CreateShaderResourceView(m_PositionBuffer, &srvd, &m_PositionSRV);

	//アンオーダーアクセスビュー
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavd;
	ZeroMemory(&uavd, sizeof(uavd));
	uavd.Format = DXGI_FORMAT_UNKNOWN;
	uavd.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavd.Buffer.FirstElement = 0;
	uavd.Buffer.NumElements = 4;
	Renderer::GetDevice()->CreateUnorderedAccessView(m_ResultBuffer, &uavd, &m_ResultUAV);



}

void SandParticle::Uninit()
{
	
}

void SandParticle::Update()
{
	//パーティクルの情報をバッファに入れる
	D3D11_MAPPED_SUBRESOURCE ms;
	Renderer::GetDeviceContext()->Map(m_ParticleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	PARTICLE* pBufType = (PARTICLE*)ms.pData;
	memcpy(ms.pData, m_Particle, sizeof(PARTICLE) * PARTICLE_MAX);
	Renderer::GetDeviceContext()->Unmap(m_ParticleBuffer, 0);

	//コンピュートシェーダー実行
	ID3D11ShaderResourceView* srv[1] = { m_ParticleSRV };
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, srv);
	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ResultUAV, 0);
	Renderer::GetDeviceContext()->Dispatch(256, 1, 1);

	

	//パーティクル発射
	for (int i = 0; i < PARTICLE_MAX; i++) {
		if (!m_Particle[i].Enable) {
			m_Particle[i].Enable = true;

			m_Particle[i].Pos = GetPos();
			m_Particle[i].Vel.x = (rand() % 100 - 50) / 500.0f;
			m_Particle[i].Vel.y = (rand() % 100 + 50) / 500.0f;
			m_Particle[i].Vel.z = (rand() % 100 - 50) / 500.0f;

			m_Particle[i].Life = 60;

			break;
		}
	}
	for (int i = 0; i < PARTICLE_MAX; i++) {
		if (m_Particle[i].Enable) {
			//重力
			m_Particle[i].Vel.y += -0.01f;

			m_Particle[i].Pos.x += m_Particle[i].Vel.x;
			m_Particle[i].Pos.y += m_Particle[i].Vel.y;
			m_Particle[i].Pos.z += m_Particle[i].Vel.z;

			m_Particle[i].Life--;
			if (m_Particle[i].Life < 0) {
				m_Particle[i].Enable = false;
			}
		}

	}

}

void SandParticle::Draw()
{
	TitleCamera* camera = Scene::GetInstance()->GetScene<Title>()->GetGameObject<TitleCamera>();
	XMMATRIX view = camera->GetViewMatrix();

	//ビューの逆行列
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);	//逆行列
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRot().x, GetRot().y, GetRot().z);
	trans = XMMatrixTranslation(GetPos().x, GetPos().y, GetPos().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン設定
	Renderer::GetDeviceContext()->Draw(4, 0);


}
