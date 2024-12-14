#include "transform2DComponent.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "titleCamera.h"
#include "game.h"
#include "title.h"

void Transform2DComponent::Init()
{
	if (m_TextureData == L"none") return;
	if (m_isBillBoard)
	{
		//頂点バッファ生成
		D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


		//テクスチャ読み込み
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICFile(m_TextureData, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
		assert(m_Texture);

		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
			"shader\\unlitTextureVS.cso");

		Renderer::CreatePixelShader(&m_PixelShader,
			"shader\\unlitTexturePS.cso");
	}
	else
	{
		//頂点バッファ生成
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


		//テクスチャ読み込み
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICFile(m_TextureData, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
		assert(m_Texture);

		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
			"shader\\unlitTextureVS.cso");

		Renderer::CreatePixelShader(&m_PixelShader,
			"shader\\unlitTexturePS.cso");


	}

}

void Transform2DComponent::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Transform2DComponent::Update()
{

}

void Transform2DComponent::Draw()
{
	if (m_isBillBoard)
	{
		if (m_isZBuff)
		{
			//入力レイアウト設定
			Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

			//シェーダー設定
			Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
			Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

			XMMATRIX view;

			//カメラのビューマトリクス取得
			if (m_isTitle)
			{
				TitleCamera* titlecamera = Scene::GetInstance()->GetScene<Title>()->GetGameObject<TitleCamera>();
				view = titlecamera->GetViewMatrix();
			}
			else
			{
				Camera* camera = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Camera>();
				view = camera->GetViewMatrix();
			}

			//ビューの逆行列
			XMMATRIX invView;
			invView = XMMatrixInverse(nullptr, view);	//逆行列
			invView.r[3].m128_f32[0] = 0.0f;
			invView.r[3].m128_f32[1] = 0.0f;
			invView.r[3].m128_f32[2] = 0.0f;


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

			//Zバッファ無効
			Renderer::SetDepthEnable(false);

			//ワールドマトリクス設定
			XMMATRIX world, scale, rot, trans;
			scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
			trans = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			//ポリゴン設定
			Renderer::GetDeviceContext()->Draw(4, 0);

			//Zバッファ有効
			Renderer::SetDepthEnable(true);

		}
		else
		{
			//頂点データ書き換え
			D3D11_MAPPED_SUBRESOURCE msr;
			Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

			vertex[0] = m_Vertex[0];
			vertex[1] = m_Vertex[1];
			vertex[2] = m_Vertex[2];
			vertex[3] = m_Vertex[3];

			Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

			//入力レイアウト設定
			Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

			//シェーダー設定
			Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
			Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

			//カメラのビューマトリクス取得
			Camera* camera = Scene::GetInstance()->GetScene<Game>()->GetGameObject<Camera>();
			XMMATRIX view = camera->GetViewMatrix();

			//ビューの逆行列
			XMMATRIX invView;
			invView = XMMatrixInverse(nullptr, view);	//逆行列
			invView.r[3].m128_f32[0] = 0.0f;
			invView.r[3].m128_f32[1] = 0.0f;
			invView.r[3].m128_f32[2] = 0.0f;

			//ワールドマトリクス設定
			XMMATRIX world, scale, rot, trans;
			scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
			trans = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
			world = scale * invView * trans;
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
	}
	else
	{
		//入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		//シェーダー設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		//ワールドマトリクス設定
		XMMATRIX world, scale, rot, trans;
		scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		rot = XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
		trans = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
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
}

void Transform2DComponent::SetVertex(VERTEX_3D vertex[4])
{
	m_Vertex[0] = vertex[0];
	m_Vertex[1] = vertex[1];
	m_Vertex[2] = vertex[2];
	m_Vertex[3] = vertex[3];
}

void Transform2DComponent::SetPosSize(float posx, float posy, float sizex, float sizey)
{
	m_VertexPos.x = posx;
	m_VertexPos.y = posy;
	m_VertexSize.x = sizex;
	m_VertexSize.y = sizey;
}

void Transform2DComponent::SetAnimation(float animx, float animy, float animsizex, float animsizey)
{
	m_AnimX = animx;
	m_AnimY = animy;
	m_AnimSizeX = animsizex;
	m_AnimSizeY = animsizey;
}
