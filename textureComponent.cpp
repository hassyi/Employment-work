#include "textureComponent.h"

void TextureComponent::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(m_VertexPos.x, m_VertexPos.y, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(m_VertexPos.x + m_VertexSize.x, m_VertexPos.y, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(m_VertexPos.x, m_VertexPos.y + m_VertexSize.y, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(m_VertexPos.x + m_VertexSize.x, m_VertexPos.y + m_VertexSize.y, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

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

void TextureComponent::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void TextureComponent::Update()
{

}

void TextureComponent::Draw()
{
	if (m_IsDisitDisplay)
	{
		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = XMFLOAT3(m_VertexPos.x + m_Disit, m_VertexPos.y, 0.0f);
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = XMFLOAT2(m_AnimX, m_AnimY);

		vertex[1].Position = XMFLOAT3(m_VertexPos.x + m_VertexSize.x + m_Disit, m_VertexPos.y, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = XMFLOAT2(m_AnimX + m_AnimSizeX, m_AnimY);

		vertex[2].Position = XMFLOAT3(m_VertexPos.x + m_Disit, m_VertexPos.y + m_VertexSize.y, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = XMFLOAT2(m_AnimX, m_AnimY + m_AnimSizeY);

		vertex[3].Position = XMFLOAT3(m_VertexPos.x + m_VertexSize.x + m_Disit, m_VertexPos.y + m_VertexSize.y, 0.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = XMFLOAT2(m_AnimX + m_AnimSizeX, m_AnimY + m_AnimSizeY);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		//入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		//シェーダー設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		//マトリクス設定
		Renderer::SetWorldViewProjection2D();

		//頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		//マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, m_Alpha);
		material.TextureEnable = true;
		Renderer::SetMaterial(material);

		//テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		//プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//ポリゴン設定
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
	else 
	{
		////頂点データ書き換え
		//D3D11_MAPPED_SUBRESOURCE msr;
		//Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		//VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//vertex[0].Position = XMFLOAT3(m_VertexPos.x, m_VertexPos.y, 0.0f);
		//vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//vertex[0].TexCoord = XMFLOAT2(m_AnimX, m_AnimY);

		//vertex[1].Position = XMFLOAT3(m_VertexPos.x + m_VertexSize.x, m_VertexPos.y, 0.0f);
		//vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//vertex[1].TexCoord = XMFLOAT2(m_AnimX + m_AnimSizeX, m_AnimY);

		//vertex[2].Position = XMFLOAT3(m_VertexPos.x, m_VertexPos.y + m_VertexSize.y, 0.0f);
		//vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//vertex[2].TexCoord = XMFLOAT2(m_AnimX, m_AnimY + m_AnimSizeY);

		//vertex[3].Position = XMFLOAT3(m_VertexPos.x + m_VertexSize.x, m_VertexPos.y + m_VertexSize.y, 0.0f);
		//vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//vertex[3].TexCoord = XMFLOAT2(m_AnimX + m_AnimSizeX, m_AnimY + m_AnimSizeY);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		//入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		//シェーダー設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		//マトリクス設定
		Renderer::SetWorldViewProjection2D();

		//頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		//マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, m_Alpha);
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
