#include "manager.h"
#include "wave.h"


void Wave::Init()
{
	//頂点バッファ生成
	{
		for (int x = 0; x < SIZE_NUM; x++)
		{
			for (int z = 0; z < SIZE_NUM; z++)
			{
				m_Vertex[x][z].Position = XMFLOAT3((x - 10) * 5.0f, 0.0f, (z - 10) * -5.0f);
				m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[x][z].TexCoord = XMFLOAT2(x, z);
			}
		}

		//法線ベクトル算出
		for (int x = 1; x < SIZE_NUM - 1; x++)
		{
			for (int z = 1; z < SIZE_NUM - 1; z++)
			{
				XMFLOAT3 vx, vz, vn;

				vx.x = m_Vertex[x + 1][z].Position.x - m_Vertex[x - 1][z].Position.x;
				vx.y = m_Vertex[x + 1][z].Position.y - m_Vertex[x - 1][z].Position.y;
				vx.z = m_Vertex[x + 1][z].Position.z - m_Vertex[x - 1][z].Position.z;

				vz.x = m_Vertex[x][z - 1].Position.x - m_Vertex[x][z + 1].Position.x;
				vz.y = m_Vertex[x][z - 1].Position.y - m_Vertex[x][z + 1].Position.y;
				vz.z = m_Vertex[x][z - 1].Position.z - m_Vertex[x][z + 1].Position.z;

				//外積
				vn.x = vz.y * vx.z - vz.z * vx.y;
				vn.y = vz.z * vx.x - vz.x * vx.z;
				vn.z = vz.x * vx.y - vz.y * vx.x;

				//正規化
				float len = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
				vn.x /= len;
				vn.y /= len;
				vn.z /= len;

				m_Vertex[x][z].Normal = vn;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * SIZE_NUM * SIZE_NUM;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	//インデックスバッファ
	{
		unsigned int index[((SIZE_NUM + 1) * 2) * (SIZE_NUM - 1) - 2];

		int i = 0;
		for (int x = 0; x < SIZE_NUM - 1; x++)
		{
			for (int z = 0; z < SIZE_NUM; z++)
			{
				index[i] = x * SIZE_NUM + z;
				i++;

				index[i] = (x + 1) * SIZE_NUM + z;
				i++;
			}
			if (x == SIZE_NUM - 2)
				break;
			index[i] = (x + 1) * SIZE_NUM + (SIZE_NUM - 1);
			i++;

			index[i] = (x + 1) * SIZE_NUM;
			i++;
		}
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * (((SIZE_NUM + 1) * 2) * (SIZE_NUM - 1) - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\wave.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	//環境マッピング
	LoadFromWICFile(L"asset\\texture\\sky.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_EnvTexture);
	assert(m_EnvTexture);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\waveVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\wavePS.cso");


}

void Wave::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_Texture->Release();
	m_EnvTexture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Wave::Update()
{
	float amplitude = 1.0f;
	float waveLength = 2.0f;
	float waveCycle = 2.0f;
	for (int x = 0; x < SIZE_NUM; x++)
	{
		for (int z = 0; z < SIZE_NUM; z++)
		{
			float dx = m_Vertex[x][z].Position.x - m_Vertex[0][0].Position.x;
			float dz = m_Vertex[x][z].Position.z - m_Vertex[0][0].Position.z;
			float length = sqrtf(dx * dx + dz * dz);
			
			m_Vertex[x][z].Position.y = amplitude * sinf(2.0f * XM_PI * (length / waveLength - m_Time / waveCycle));

		}
	}

	m_Time += 1.0f / 60.0f;


	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	memcpy(vertex, m_Vertex, sizeof(VERTEX_3D) * SIZE_NUM * SIZE_NUM);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

}

void Wave::Draw()
{
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

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_EnvTexture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン設定
	//Renderer::GetDeviceContext()->Draw(21 * 21, 0);
	Renderer::GetDeviceContext()->DrawIndexed(((SIZE_NUM + 1) * 2) * (SIZE_NUM - 1) - 2, 0, 0);

}
