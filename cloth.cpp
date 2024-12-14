#include "cloth.h"

void Cloth::Init()
{
	{
		//頂点の初期化
		for (int y = 0; y < VERTEX_Y; y++)
		{
			for (int x = 0; x <= VERTEX_X; x++)
			{
				m_Vertex[y][x].vertex.Position = XMFLOAT3(3.0f * (x - VERTEX_X / 2), -3.0f * (y - VERTEX_Y / 2), 0.0f);
				m_Vertex[y][x].vertex.Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
				m_Vertex[y][x].vertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[y][x].vertex.TexCoord = XMFLOAT2(x / VERTEX_X, y / VERTEX_Y);
				m_Vertex[y][x].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
				m_Vertex[y][x].asccel = XMFLOAT3(0.0f, 0.0f, 0.0f);
				m_Vertex[y][x].resultant = XMFLOAT3(0.0f, 0.0f, 0.0f);
				m_Vertex[y][x].gravity = XMFLOAT3(0.0f, SPRING_MASS * -9.8f, 0.0f);
				m_Vertex[y][x].OnLook = (x == 0) && (y == 0 || y == VERTEX_Y);
			}
		}

		//バネの初期化
		float xx, yy, zz;
		int count = 0;
		for (int y = 0; y <= VERTEX_Y; y++)
		{
			for (int x = 0; x <= VERTEX_X; x++)
			{
				if (x < VERTEX_X)
				{
					m_Spring[count].p1.x = x;
					m_Spring[count].p1.y = y;
					m_Spring[count].p2.x = x + 1;
					m_Spring[count].p2.y = y;
					xx = m_Vertex[y][x].vertex.Position.x - m_Vertex[y][x + 1].vertex.Position.x;
					yy = m_Vertex[y][x].vertex.Position.y - m_Vertex[y][x + 1].vertex.Position.y;
					zz = m_Vertex[y][x].vertex.Position.z - m_Vertex[y][x + 1].vertex.Position.z;
					m_Spring->length = sqrtf(xx * xx + yy * yy + zz * zz);
					count++;
				}
				if (y < VERTEX_Y)
				{
					m_Spring[count].p1.x = x;
					m_Spring[count].p1.y = y;
					m_Spring[count].p2.x = x;
					m_Spring[count].p2.y = y + 1;
					xx = m_Vertex[y][x].vertex.Position.x - m_Vertex[y + 1][x].vertex.Position.x;
					yy = m_Vertex[y][x].vertex.Position.y - m_Vertex[y + 1][x].vertex.Position.y;
					zz = m_Vertex[y][x].vertex.Position.z - m_Vertex[y + 1][x].vertex.Position.z;
					m_Spring->length = sqrtf(xx * xx + yy * yy + zz * zz);
					count++;
				}
				if (x < VERTEX_X && y < VERTEX_Y)
				{
					m_Spring[count].p1.x = x;
					m_Spring[count].p1.y = y;
					m_Spring[count].p2.x = x + 1;
					m_Spring[count].p2.y = y + 1;
					xx = m_Vertex[y][x].vertex.Position.x - m_Vertex[y + 1][x + 1].vertex.Position.x;
					yy = m_Vertex[y][x].vertex.Position.y - m_Vertex[y + 1][x + 1].vertex.Position.y;
					zz = m_Vertex[y][x].vertex.Position.z - m_Vertex[y + 1][x + 1].vertex.Position.z;
					m_Spring->length = sqrtf(xx * xx + yy * yy + zz * zz);
					count++;
				}
				if (x > 0 && y < VERTEX_Y)
				{
					m_Spring[count].p1.x = x;
					m_Spring[count].p1.y = y;
					m_Spring[count].p2.x = x - 1;
					m_Spring[count].p2.y = y + 1;
					xx = m_Vertex[y][x].vertex.Position.x - m_Vertex[y + 1][x - 1].vertex.Position.x;
					yy = m_Vertex[y][x].vertex.Position.y - m_Vertex[y + 1][x - 1].vertex.Position.y;
					zz = m_Vertex[y][x].vertex.Position.z - m_Vertex[y + 1][x - 1].vertex.Position.z;
					m_Spring->length = sqrtf(xx * xx + yy * yy + zz * zz);
					count++;
				}
			}
		}

		//法線ベクトル
		for (int y = 1; y < VERTEX_Y; y++)
		{
			for (int x = 1; x < VERTEX_X; x++)
			{
				XMFLOAT3 vx, vy, vn;

				vx.x = m_Vertex[y][x + 1].vertex.Position.x - m_Vertex[y][x - 1].vertex.Position.x;
				vx.y = m_Vertex[y][x + 1].vertex.Position.y - m_Vertex[y][x - 1].vertex.Position.y;
				vx.z = m_Vertex[y][x + 1].vertex.Position.z - m_Vertex[y][x - 1].vertex.Position.z;

				vy.x = m_Vertex[y - 1][x].vertex.Position.x - m_Vertex[y + 1][x].vertex.Position.x;
				vy.y = m_Vertex[y - 1][x].vertex.Position.y - m_Vertex[y + 1][x].vertex.Position.y;
				vy.z = m_Vertex[y - 1][x].vertex.Position.z - m_Vertex[y + 1][x].vertex.Position.z;

				//外積
				vn.x = vy.y * vx.z - vy.z * vx.y;
				vn.y = vy.z * vx.x - vy.x * vx.z;
				vn.z = vy.x * vx.y - vy.y * vx.x;

				float length = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
				vn.x /= length;
				vn.y /= length;
				vn.z /= length;

				m_Vertex[y][x].vertex.Normal = vn;
			}
		}
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * VERTEX_X * VERTEX_Y;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	//インデックスバッファ
	{
		unsigned int index[((VERTEX_X + 1) * 2) * (VERTEX_Y - 1) - 2];

		int i = 0;
		for (int x = 0; x < VERTEX_X; x++)
		{
			for (int y = 0; y < VERTEX_Y; y++)
			{
				index[i] = x * VERTEX_X + y;
				i++;

				index[i] = (x - 1) * VERTEX_Y + y;
				i++;
			}
			if (x == VERTEX_Y - 2)
				break;

			index[i] = (x + 1) * VERTEX_X + (VERTEX_Y - 1);
			i++;

			index[i] = (x + 1) * VERTEX_Y;
			i++;
		}
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * (((VERTEX_X + 1) * 2) * (VERTEX_Y - 1) - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	SetPos(XMFLOAT3(-2.0f, 0.5f, 0.0f));
	SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\field000.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_WindForce = XMFLOAT3(40.0f, 20.0f, 30.0f);

}

void Cloth::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Cloth::Update()
{
	XMFLOAT3 vec_spr;				//頂点の2点間のベクトル
	XMFLOAT3 resistance;			//抵抗力ベクトル
	XMFLOAT3 windforce;				//風力ベクトル
	float	f1, f2;					// バネが頂点に与える力
	int		x1, y1, x2, y2;			// バネ端にある頂点のインデックス（横、縦別２個分）
	XMFLOAT3 force1, force2;
	float length;
	float xx, yy, zz;
	int count;
	int x, y;

	//頂点処理
	for (y = 0; y < VERTEX_Y; y++)
	{
		for (x = 0; x < VERTEX_X; x++)
		{
			m_Vertex[y][x].resultant.x = 0;
			m_Vertex[y][x].resultant.y = 0;
			m_Vertex[y][x].resultant.z = 0;

			if (!m_Vertex[y][x].OnLook)
			{
				//重力
				m_Vertex[y][x].gravity = XMFLOAT3(0.0f, -9.8f * SPRING_MASS, 0.0f);
				//抵抗力
				resistance.x = -m_Vertex[y][x].vel.x * ATT_COEFFICIENT;
				resistance.y = -m_Vertex[y][x].vel.y * ATT_COEFFICIENT;
				resistance.z = -m_Vertex[y][x].vel.z * ATT_COEFFICIENT;
				//風力
				windforce = m_WindForce;
				//合力
				m_Vertex[y][x].resultant.x = m_Vertex[y][x].gravity.x + resistance.x + windforce.x;
				m_Vertex[y][x].resultant.y = m_Vertex[y][x].gravity.y + resistance.y + windforce.y;
				m_Vertex[y][x].resultant.z = m_Vertex[y][x].gravity.z + resistance.z + windforce.z;
			}
		}
	}

	//バネの処理
	for (y = 0; y < SPRING_NUM; y++)
	{
		//バネの参照頂点を取得
		x1 = m_Spring[y].p1.x;
		y1 = m_Spring[y].p1.y;
		x2 = m_Spring[y].p1.x;
		y2 = m_Spring[y].p1.y;
		//2個の頂点間のベクトルを求める
		vec_spr.x = m_Vertex[y1][x1].vertex.Position.x - m_Vertex[y2][x2].vertex.Position.x;
		vec_spr.y = m_Vertex[y1][x1].vertex.Position.y - m_Vertex[y2][x2].vertex.Position.y;
		vec_spr.z = m_Vertex[y1][x1].vertex.Position.z - m_Vertex[y2][x2].vertex.Position.z;
		//頂点間の距離
		length = sqrtf(vec_spr.x * vec_spr.x + vec_spr.y * vec_spr.y + vec_spr.z * vec_spr.z);
		//距離、自然長、バネ係数からかかる力を求める
		f1 = -40.0f * (length - m_Spring[y].length);
		f2 = -f1;
		//ベクトルに力をかける
		force1.x = f1 * vec_spr.x / length;
		force1.y = f1 * vec_spr.y / length;
		force1.z = f1 * vec_spr.z / length;

		force2.x = f2 * vec_spr.x / length;
		force2.y = f2 * vec_spr.y / length;
		force2.z = f2 * vec_spr.z / length;

		//求めた力を加える
		m_Vertex[y1][x1].resultant.x += force1.x;
		m_Vertex[y1][x1].resultant.y += force1.y;
		m_Vertex[y1][x1].resultant.z += force1.z;

		m_Vertex[y2][x2].resultant.x += force2.x;
		m_Vertex[y2][x2].resultant.y += force2.y;
		m_Vertex[y2][x2].resultant.z += force2.z;
	}

	for (y = 0; y < VERTEX_Y; y++)
	{
		for (x = 0; x < VERTEX_X; x++)
		{
			if (m_Vertex[y][x].OnLook) continue;

			//合力と質量から加速度を求める
			m_Vertex[y][x].asccel.x = m_Vertex[y][x].resultant.x / SPRING_MASS;
			m_Vertex[y][x].asccel.y = m_Vertex[y][x].resultant.y / SPRING_MASS;
			m_Vertex[y][x].asccel.z = m_Vertex[y][x].resultant.z / SPRING_MASS;
			//速度に加速度を加える
			m_Vertex[y][x].vel.x += m_Vertex[y][x].asccel.x * DELTA_TIME;
			m_Vertex[y][x].vel.y += m_Vertex[y][x].asccel.y * DELTA_TIME;
			m_Vertex[y][x].vel.z += m_Vertex[y][x].asccel.z * DELTA_TIME;
			//座標を速度分動かす
			m_Vertex[y][x].vertex.Position.x += m_Vertex[y][x].vel.x * DELTA_TIME;
			m_Vertex[y][x].vertex.Position.y += m_Vertex[y][x].vel.y * DELTA_TIME;
			m_Vertex[y][x].vertex.Position.z += m_Vertex[y][x].vel.z * DELTA_TIME;
		}
	}

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	memcpy(vertex, m_Vertex, sizeof(VERTEX_3D) * VERTEX_X * VERTEX_Y);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}

void Cloth::Draw()
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

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン設定
	//Renderer::GetDeviceContext()->Draw(((VERTEX_X + 1) * 2) * (VERTEX_Y - 1) - 2, 0);
	Renderer::GetDeviceContext()->DrawIndexed(((VERTEX_X + 1) * 2) * (VERTEX_Y - 1) - 2, 0, 0);
}
