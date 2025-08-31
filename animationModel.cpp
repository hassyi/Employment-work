#include "main.h"
#include "renderer.h"
#include "animationModel.h"
#include <map>

void AnimationModel::Draw()
{
	int i = 0;
	BONE_MATRICES* boneMatrix = new BONE_MATRICES;
	for (int b = 0; b < 255; b++)
	{
		XMFLOAT4X4 matrix = {};

		matrix._11 = 0.0f; matrix._12 = 0.0f; matrix._13 = 0.0f; matrix._14 = 0.0f;
		matrix._21 = 0.0f; matrix._22 = 0.0f; matrix._23 = 0.0f; matrix._24 = 0.0f;
		matrix._31 = 0.0f; matrix._32 = 0.0f; matrix._33 = 0.0f; matrix._34 = 0.0f;
		matrix._41 = 0.0f; matrix._42 = 0.0f; matrix._43 = 0.0f; matrix._44 = 0.0f;

		boneMatrix->BoneMatrix[b] = matrix;
	}

	for (const std::pair<std::string, BONE> bone : m_Bone)
	{
		if (i >= 255) break;

		const aiMatrix4x4& aiMatrix = bone.second.Matrix;
		XMFLOAT4X4 matrix = {};

		matrix._11 = aiMatrix.a1; matrix._12 = aiMatrix.b1; matrix._13 = aiMatrix.c1; matrix._14 = aiMatrix.d1;
		matrix._21 = aiMatrix.a2; matrix._22 = aiMatrix.b2; matrix._23 = aiMatrix.c2; matrix._24 = aiMatrix.d2;
		matrix._31 = aiMatrix.a3; matrix._32 = aiMatrix.b3; matrix._33 = aiMatrix.c3; matrix._34 = aiMatrix.d3;
		matrix._41 = aiMatrix.a4; matrix._42 = aiMatrix.b4; matrix._43 = aiMatrix.c4; matrix._44 = aiMatrix.d4;

		//転置
		XMMATRIX matrixTranspose = XMMatrixTranspose(XMLoadFloat4x4(&matrix));
		XMStoreFloat4x4(&matrix, matrixTranspose);

		boneMatrix->BoneMatrix[i] = matrix;
		i++;
	}

	boneMatrix->VertexCount =  static_cast<UINT>(m_VertexCount);
	SetBoneMatrices(boneMatrix);


	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(nullptr);

	Renderer::GetDeviceContext()->CSSetShader(m_SkinCS, NULL, 0);
	Renderer::GetDeviceContext()->VSSetShader(m_SkinVS, NULL, 0);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		//シェーダーへデータを送信
		Renderer::GetDeviceContext()->CSSetConstantBuffers(0, 1, &m_ConstantBuffer);
		Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &m_BeforeVertexSRV[m]);
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_AfterVertexUAV[m], NULL);

		UINT numTheradGroup = (mesh->mNumVertices + 255) / 256;
		Renderer::GetDeviceContext()->Dispatch(numTheradGroup, 1, 1);

		//uavの再設定によるリソースバリア
		ID3D11UnorderedAccessView* nulluav = nullptr;
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &nulluav, nullptr);

		// マテリアル設定
		aiString texture;
		aiColor3D diffuse;
		float opacity;

		aiMaterial* aimaterial = m_AiScene->mMaterials[mesh->mMaterialIndex];
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture);
		aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aimaterial->Get(AI_MATKEY_OPACITY, opacity);

		if (texture == aiString(""))
		{
			material.TextureEnable = false;
		}
		else
		{
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[texture.data]);
			material.TextureEnable = true;
		}

		material.Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, opacity - m_Alpha);
		material.Ambient = material.Diffuse;
		Renderer::SetMaterial(material);

		// 頂点バッファ設定
		//UINT stride = sizeof(VERTEX_3D);
		//UINT offset = 0;
		//Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);
		//ConmputeShaderでのスキニング処理後の頂点設定
		Renderer::GetDeviceContext()->VSSetShaderResources(0, 1, &m_SkinningUpdateSRV[m]);

		// インデックスバッファ設定
		Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// ポリゴン描画
		Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}

	delete boneMatrix;
}


void AnimationModel::Load( const char *FileName )
{
	const std::string modelPath( FileName );

	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];


	//変形後頂点配列生成
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

	std::map<std::string, int> boneNameIndex;
	int boneCount = 0;

	//再帰的にボーン生成
	CreateBone(m_AiScene->mRootNode, boneNameIndex, boneCount);


	//GPU用のコンピュートシェーダーと頂点シェーダーの生成
	Renderer::CreateComputeShader(&m_SkinCS, "shader\\skinningCS.cso");
	Renderer::CreateVertexShader(&m_SkinVS, nullptr, "shader\\skinningVS.cso");

	m_VertexCount = 0;

	std::vector<ANIMATION_VERTEX> vertexList;
	std::vector<VERTEX> afterList;
	std::vector<unsigned int> indexAll;

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// 頂点バッファ生成
		{
			ANIMATION_VERTEX* animVertex = new ANIMATION_VERTEX[mesh->mNumVertices];
			m_VertexCount += mesh->mNumVertices;

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				animVertex[v].Position = XMFLOAT4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.0f);
				animVertex[v].Normal = XMFLOAT4(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, 1.0f);
				animVertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				animVertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				for (int b = 0; b < 4; b++)
				{
					animVertex[v].BoneIndex[b] = 0;
					animVertex[v].BoneWeight[b] = 0.0f;
				}
			}

			//変形後頂点データ初期化
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				DEFORM_VERTEX deformVertex;
				deformVertex.Position = mesh->mVertices[v];
				deformVertex.Normal = mesh->mNormals[v];
				deformVertex.BoneNum = 0;

				for (unsigned int b = 0; b < 4; b++)
				{
					deformVertex.BoneName[b] = "";
					deformVertex.BoneWeight[b] = 0.0f;
				}

				m_DeformVertex[m].push_back(deformVertex);
			}

			//ボーンデータ初期化
			for (unsigned int b = 0; b < mesh->mNumBones; b++)
			{
				aiBone* bone = mesh->mBones[b];

				m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

				//変形後頂点にボーンデータ格納
				for (unsigned int w = 0; w < bone->mNumWeights; w++)
				{
					aiVertexWeight weight = bone->mWeights[w];

					int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

					m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
					m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
					m_DeformVertex[m][weight.mVertexId].BoneNum++;

					assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);
				}
			}

			//ボーンの番号を名前から算出
			unsigned int num = m_DeformVertex[m].size();
			for (unsigned int i = 0; i < num; i++)
			{
				int b = 0;
				for (auto bone : m_Bone)
				{
					for (int v = 0; v < 4; v++)
					{
						if (bone.first == m_DeformVertex[m][i].BoneName[v])
						{
							boneNameIndex[bone.first] = b;
						}
					}
					b++;
				}
			}

			//ボーンの名前が同じものに登録しておいた番号と同じボーンの影響度合いを格納
			for (unsigned int v = 0; v < num; v++)
			{
				for (auto bone : boneNameIndex)
				{
					for (int w = 0; w < 4; w++)
					{
						if (m_DeformVertex[m][v].BoneName[w] == bone.first)
						{
							animVertex[v].BoneIndex[w] = bone.second;
							animVertex[v].BoneWeight[w] = m_DeformVertex[m][v].BoneWeight[w];
						}
					}
				}
			}

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				ANIMATION_VERTEX& v = animVertex[i];

				float wight = v.BoneWeight[0] + v.BoneWeight[1] + v.BoneWeight[2] + v.BoneWeight[3];
				vertexList.push_back(animVertex[i]);
				if (wight > 1.1f)
				{
					wight = 0;
				}
			}

			//変形前頂点バッファ作成
			CreateBeforeVertexBuffer(mesh->mNumVertices, animVertex, m);
			//変形前の頂点バッファのリソースビュー作成
			CreateBeforeSRV(mesh->mNumVertices, m);
			VERTEX* vertex = new VERTEX[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = XMFLOAT4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.0f);
				vertex[v].Normal = XMFLOAT4(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, 1.0f);
				vertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				VERTEX& ver = vertex[v];
				afterList.push_back(ver);
			}

			//計算後の頂点格納用バッファ作成
			CreateAfterVertexBuffer(mesh->mNumVertices, vertex, m);
			CreateAfterVertexSRV(mesh->mNumVertices, m);
			CreateUAV(mesh->mNumVertices, m);
		}


		// インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
				indexAll.push_back(index[f * 3 + 0]);
				indexAll.push_back(index[f * 3 + 1]);
				indexAll.push_back(index[f * 3 + 2]);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

			delete[] index;
		}
	}

	//テクスチャ読み込み
	for(unsigned int i = 0; i < m_AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = m_AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;

		// テクスチャ読み込み
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICMemory(aitexture->pcData, aitexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		assert(texture);

		m_Texture[aitexture->mFilename.data] = texture;
	}

	CreateConstantBuffer();

}




void AnimationModel::Uninit()
{

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	delete[] m_DeformVertex;


	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_Texture)
	{
		pair.second->Release();
	}



	aiReleaseImport(m_AiScene);


	for (std::pair<const std::string, const aiScene*> pair : m_Animation)
	{
		aiReleaseImport(pair.second);
	}

}


void AnimationModel::LoadAnimation(const char* FileName, const char* Name)
{
	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);
}

void AnimationModel::CreateBone(aiNode* node, std::map<std::string, int>& boneNameIndex, int& boneCount)
{
	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n], boneNameIndex, boneCount);
	}
}

void AnimationModel::UpdateAnimation(const char* AnimationName1, int Frame1,
							const char* AnimationName2, int Frame2, 
							float BlendRatio)
{
	if (m_Animation.count(AnimationName1) == 0)
		return;
	if (m_Animation.count(AnimationName2) == 0)
		return;

	if (!m_Animation[AnimationName1]->HasAnimations())
		return;
	if (!m_Animation[AnimationName2]->HasAnimations())
		return;

	//アニメーションデータからボーンマトリクス算出
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];
	aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];


	for (auto pair : m_Bone)
	{
		BONE* bone = &m_Bone[pair.first];

		aiNodeAnim* nodeAnim1 = nullptr;
		aiNodeAnim* nodeAnim2 = nullptr;

		for (unsigned int c = 0; c < animation1->mNumChannels; c++)
		{
			if (animation1->mChannels[c]->mNodeName == aiString(pair.first))
			{
				nodeAnim1 = animation1->mChannels[c];
				break;
			}
		}

		for (unsigned int c = 0; c < animation2->mNumChannels; c++)
		{
			if (animation2->mChannels[c]->mNodeName == aiString(pair.first))
			{
				nodeAnim2 = animation2->mChannels[c];
				break;
			}
		}

		int f;

		aiQuaternion rot1;
		aiVector3D pos1;

		if (nodeAnim1)
		{
			f = Frame1 % nodeAnim1->mNumRotationKeys;	//簡易実装
			rot1 = nodeAnim1->mRotationKeys[f].mValue;

			f = Frame1 % nodeAnim1->mNumPositionKeys;	//簡易実装
			pos1 = nodeAnim1->mPositionKeys[f].mValue;
		}

		aiQuaternion rot2;
		aiVector3D pos2;

		if (nodeAnim2)
		{
			f = Frame2 % nodeAnim2->mNumRotationKeys;	//簡易実装
			rot2 = nodeAnim2->mRotationKeys[f].mValue;

			f = Frame2 % nodeAnim2->mNumPositionKeys;	//簡易実装
			pos2 = nodeAnim2->mPositionKeys[f].mValue;
		}

		aiVector3D pos;
		pos = pos1 * (1.0f - BlendRatio) + pos2 * BlendRatio;		//線形補間

		aiQuaternion rot;	
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRatio);		//球面線形補間

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}


	//再帰的にボーンマトリクスを更新
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	return;
}

void AnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//マトリクスの乗算順番に注意
	aiMatrix4x4 worldMatrix;

	worldMatrix *= matrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;

	//これで部位のマトリクスは取れる
	if (strcmp(node->mName.C_Str(), "mixamorig:RightHand") == 0)
	{

		XMFLOAT4X4 mat = {};

		mat._11 = 0.0f; mat._12 = 0.0f; mat._13 = 0.0f; mat._14 = 0.0f;
		mat._21 = 0.0f; mat._22 = 0.0f; mat._23 = 0.0f; mat._24 = 0.0f;
		mat._31 = 0.0f; mat._32 = 0.0f; mat._33 = 0.0f; mat._34 = 0.0f;
		mat._41 = 0.0f; mat._42 = 0.0f; mat._43 = 0.0f; mat._44 = 0.0f;

		aiMatrix4x4& aiMat = bone->Matrix;

		mat._11 = aiMat.a1; mat._12 = aiMat.b1; mat._13 = aiMat.c1; mat._14 = aiMat.d1;
		mat._21 = aiMat.a2; mat._22 = aiMat.b2; mat._23 = aiMat.c2; mat._24 = aiMat.d2;
		mat._31 = aiMat.a3; mat._32 = aiMat.b3; mat._33 = aiMat.c3; mat._34 = aiMat.d3;
		mat._41 = aiMat.a4; mat._42 = aiMat.b4; mat._43 = aiMat.c4; mat._44 = aiMat.d4;

		// 転置
		XMMATRIX matTranspose = XMLoadFloat4x4(&mat);
		m_RightHandMatrix = matTranspose;
	}
	

	bone->Matrix *= bone->OffsetMatrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

void AnimationModel::CreateBeforeVertexBuffer(int index, ANIMATION_VERTEX* vertex, int mesh)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ANIMATION_VERTEX) * index;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.StructureByteStride = sizeof(ANIMATION_VERTEX);

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_BeforeVertexBuffer[mesh]);
}

void AnimationModel::CreateBeforeSRV(int index, int mesh)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.NumElements = index;
	SRVDesc.Buffer.FirstElement = 0;
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;

	Renderer::GetDevice()->CreateShaderResourceView(m_BeforeVertexBuffer[mesh], &SRVDesc, &m_BeforeVertexSRV[mesh]);
}

void AnimationModel::CreateAfterVertexBuffer(int index, VERTEX* vertex, int mesh)
{
	//バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX) * index;
	bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.StructureByteStride = sizeof(VERTEX);		//各要素のサイズを設定

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.pSysMem = vertex;
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_AfterVertexBuffer[mesh]);
}

void AnimationModel::CreateUAV(int index, int mesh)
{
	//csで書き出される用のUAV
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = index;

	HRESULT hr = Renderer::GetDevice()->CreateUnorderedAccessView(m_AfterVertexBuffer[mesh], &uavDesc, &m_AfterVertexUAV[mesh]);
}

void AnimationModel::CreateAfterVertexSRV(int index, int mesh)
{
	//vsで読み込まれるスキニング処理後を送るSRV
	D3D11_SHADER_RESOURCE_VIEW_DESC updateSrvDesc = {};
	updateSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	updateSrvDesc.Buffer.FirstElement = 0;
	updateSrvDesc.Buffer.NumElements = index;
	updateSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	
	Renderer::GetDevice()->CreateShaderResourceView(m_AfterVertexBuffer[mesh], &updateSrvDesc, &m_SkinningUpdateSRV[mesh]);
}

void AnimationModel::CreateConstantBuffer()
{
	// CSに渡す定数バッファの作成
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	//16バイトアライメントを確保
	size_t size = sizeof(BONE_MATRICES);
	bufferDesc.ByteWidth = (size + 15) & ~15;

	bufferDesc.StructureByteStride = 0;
	bufferDesc.MiscFlags = 0;

	Renderer::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_ConstantBuffer);
}

void AnimationModel::SetBoneMatrices(const BONE_MATRICES* matrices)
{
	Renderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, nullptr, matrices, 0, 0);
}
