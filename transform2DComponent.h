#pragma once
#include "transform.h"

class Transform2DComponent : public Transform
{
private:
	VERTEX_3D m_Vertex[4] = {};

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	const wchar_t* m_TextureData = L"none";

	XMFLOAT2 m_VertexPos = {};
	XMFLOAT2 m_VertexSize = {};

	float m_AnimX = 0.0f, m_AnimY = 0.0f;
	float m_AnimSizeX = 0.0f, m_AnimSizeY = 0.0f;

	int m_Count = 0;
	bool m_isBillBoard = false;
	bool m_isZBuff = false;
 
public:
	Transform2DComponent() = default;
	Transform2DComponent(GameObject* gameobj, const wchar_t* data) : Transform(gameobj), m_TextureData(data){}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void AddTextureData(const wchar_t* data)
	{
		m_TextureData = data;
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICFile(m_TextureData, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
		assert(m_Texture);
	}

	void SetVertex(VERTEX_3D vertex[4]);
	void SetBillBoard(bool isbillboard) { m_isBillBoard = isbillboard; }
	void SetPosSize(float posx, float posy, float sizex, float sizey);
	void SetAnimation(float animx, float animy, float animsizex, float animsizey);
	void SetZBuff(bool isZBuff) { m_isZBuff = isZBuff; }
};