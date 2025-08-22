#pragma once

#include <map>
#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "gameObjectComponent.h"

#define MESH_MAX (100)


class AnimationModel : public GameObjectComponet
{
private:
	//ボーンデータ
	struct ANIMATION_VERTEX
	{
		XMFLOAT4 Position;
		XMFLOAT4 Normal;
		XMFLOAT4 Diffuse;
		XMFLOAT2 TexCoord;

		//
		int BoneIndex[4];
		float BoneWeight[4];
	};

	//変形後頂点構造体
	struct DEFORM_VERTEX
	{
		aiVector3D Position;
		aiVector3D Normal;
		int				BoneNum;
		std::string		BoneName[4];//本来はボーンインデックスで管理するべき
		float			BoneWeight[4];
	};

	struct VERTEX
	{
		XMFLOAT4 Position;
		XMFLOAT4 Normal;
		XMFLOAT4 Diffuse;
		XMFLOAT2 TexCoord;
	};

	//ボーンマトリクス構造体
	struct BONE
	{
		aiMatrix4x4 Matrix = { 1.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 1.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 1.0f };
		aiMatrix4x4 AnimationMatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
									   0.0f, 1.0f, 0.0f, 0.0f,
									   0.0f, 0.0f, 1.0f, 0.0f,
									   0.0f, 0.0f, 0.0f, 1.0f };
		aiMatrix4x4 OffsetMatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
									   0.0f, 1.0f, 0.0f, 0.0f,
									   0.0f, 0.0f, 1.0f, 0.0f,
									   0.0f, 0.0f, 0.0f, 1.0f };
		aiMatrix4x4 WorldMatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
									   0.0f, 1.0f, 0.0f, 0.0f,
									   0.0f, 0.0f, 1.0f, 0.0f,
									   0.0f, 0.0f, 0.0f, 1.0f };
	};

	struct BONE_MATRICES
	{
		XMFLOAT4X4 BoneMatrix[256];
		UINT VertexCount;
	};

	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;

	ID3D11Buffer**	m_VertexBuffer;
	ID3D11Buffer**	m_IndexBuffer;
	ID3D11Buffer*	m_IndexAllBuffer;


	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;//変形後頂点データ
	std::unordered_map<std::string, BONE> m_Bone;//ボーンデータ（名前で参照）
	BONE_MATRICES m_BoneMatrices;

	std::unordered_map <std::string, int> m_BoneNameID;

	//GPUスキニング用バッファとSRVとUAV
	//コンピュートシェーダー
	ID3D11Buffer* m_BeforeVertexBuffer[MESH_MAX];
	ID3D11Buffer* m_AfterVertexBuffer[MESH_MAX];

	ID3D11ShaderResourceView* m_BeforeVertexSRV[MESH_MAX];
	ID3D11UnorderedAccessView* m_AfterVertexUAV[MESH_MAX];

	//頂点シェーダー
	ID3D11Buffer* m_SkinningUpdateBuffer[MESH_MAX];
	ID3D11ShaderResourceView* m_SkinningUpdateSRV[MESH_MAX];

	ID3D11Buffer* m_ConstantBuffer{};

	ID3D11VertexShader* m_SkinVS{};
	ID3D11ComputeShader* m_SkinCS{};

	int m_BoneCount = 0;
	float m_Alpha = 0.0f;
	int m_VertexCount = 0;

	XMMATRIX m_RightHandMatrix = {};

public:
	using GameObjectComponet::GameObjectComponet;

	void Load( const char *FileName );
	void Uninit() override;
	void Draw() override;
	void LoadAnimation(const char* FileName, const char* Name);
	void CreateBone(aiNode* node, std::map<std::string, int>& boneNameIndex, int& boneCount);
	void UpdateAnimation(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRatio);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

	void CreateBeforeVertexBuffer(int index, ANIMATION_VERTEX* vertex, int mesh);
	void CreateAfterVertexBuffer(int index, VERTEX* vertex, int mesh);
	void CreateBeforeSRV(int index, int mesh);

	void CreateUAV(int index, int mesh);
	void CreateAfterVertexSRV(int index, int mesh);
	void CreateConstantBuffer();

	void SetBoneMatrices(const BONE_MATRICES* matrices);

	std::unordered_map<std::string, BONE> GetBone() const { return m_Bone; }
	XMMATRIX GetRightHandMatrix() { return m_RightHandMatrix; }
};