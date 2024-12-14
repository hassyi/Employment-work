#include "manager.h"
#include "field.h"

void Field::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-50.0f, 0.0f, 50.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(50.0f, 0.0f, 50.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(20.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-50.0f, 0.0f, -50.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 20.0f);

	vertex[3].Position = XMFLOAT3(50.0f, 0.0f, -50.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(20.0f, 20.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\fieldsand.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");


}

void Field::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Field::Update()
{

}

void Field::Draw()
{
	//LIGHT light;
	//light.Enable = true;
	//light.Direction = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	//XMVECTOR lightDirection = XMLoadFloat4(&light.Direction);
	////XMVector4Normalize(XMLoadFloat4(&light.Direction));
	//lightDirection = XMQuaternionNormalize(lightDirection);
	//light.Ambient = XMFLOAT4(0.2f, 0.1f, 0.1f, 1.0f);
	//light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//light.SkyColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);					//空の色は黒(無色)
	//light.GroundColor = XMFLOAT4(0.4f, 0.0f, 0.0f, 1.0f);				//地面色は赤
	//light.GroundNormal = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);			//地面真上向き
	//XMVECTOR GroundNormal = XMLoadFloat4(&light.GroundNormal);
	//GroundNormal = XMQuaternionNormalize(GroundNormal);

	//light.Position = XMFLOAT4(0.0f, 3.0f, -1.0f, 0.0f);
	//light.PointLightParam = XMFLOAT4(10.0f, 0.0f, 0.0f, 0.0f);

	//Renderer::SetLight(light);

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
