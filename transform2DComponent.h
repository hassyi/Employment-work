#pragma once
#include "transform.h"

class Transform2DComponent : public Transform
{
private:
	VERTEX_3D m_Vertex[4] = {};

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	const wchar_t* m_TextureData = L"none";

	int m_Count = 0;
	bool m_isBillBoard = false;
 
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

};