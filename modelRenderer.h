#pragma once



// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};


// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};


// モデル構造体
struct MODEL_OBJ
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};

struct MODEL
{
	ID3D11Buffer*	VertexBuffer;
	ID3D11Buffer*	IndexBuffer;

	SUBSET*			SubsetArray;
	unsigned int	SubsetNum;
};


#include "component.h"
#include <string>
#include <unordered_map>
#include "gameObjectComponent.h"


class ModelRenderer : public GameObjectComponet
{
private:
	//static ModelRenderer* m_ModelRenderer;
	//virtual ~ModelRenderer() { delete m_ModelRenderer; }

	static std::unordered_map<std::string, MODEL*> m_ModelPool;

	static void LoadModel(const char *FileName, MODEL *Model);
	static void LoadObj( const char *FileName, MODEL_OBJ *ModelObj );
	static void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum );

	MODEL* m_Model{};

public:
	//static ModelRenderer* GetInstatnce() 
	//{
	//	if (m_ModelRenderer == nullptr) {
	//		m_ModelRenderer = new ModelRenderer;
	//	}
	//	return m_ModelRenderer;
	//}

	static void Preload( const char *FileName );
	static void UnloadAll();


	using GameObjectComponet::GameObjectComponet;

	void Load( const char *FileName );

	void Draw() override;

};