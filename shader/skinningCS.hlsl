
#include "common.hlsl"

struct AnimationVertex
{
    float4 pos; //位置座標
    float4 normal; //法線
    float4 diffuse; //頂点色
    float2 uv; //テクスチャのUV値
    
    //ボーンにかかわるステータス
    int boneIndex[4];
    float boneWeight[4];
};

cbuffer AnimationBuffer : register(b0)
{
    float4x4 boneMatrix[256]; //各ボーンの変換行列
    uint maxVertexCount; //モデルの最大頂点数
};

StructuredBuffer<AnimationVertex> intputVertexBuffer : register(t0);        //変換前の頂点バッファ

RWStructuredBuffer<SKVS_IN> outPutVertexBuffer : register(u0);                //変換された頂点を格納するバッファ




[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x;        //スレッドidを頂点idとする
    
    if (index >= maxVertexCount) return;
    
    AnimationVertex inputVertex = intputVertexBuffer[index];
    
    //スキニング計算処理
    float4x4 skinMatrix;
    skinMatrix = mul(boneMatrix[inputVertex.boneIndex[0]], inputVertex.boneWeight[0]);
    skinMatrix += mul(boneMatrix[inputVertex.boneIndex[1]], inputVertex.boneWeight[1]);
    skinMatrix += mul(boneMatrix[inputVertex.boneIndex[2]], inputVertex.boneWeight[2]);
    skinMatrix += mul(boneMatrix[inputVertex.boneIndex[3]], inputVertex.boneWeight[3]);
    
    //出力
    SKVS_IN outputVertex;
    outputVertex.Position = mul(inputVertex.pos, skinMatrix);
    
    //法線用に移動を消す
    skinMatrix[0][3] = 0.0f;
    skinMatrix[1][3] = 0.0f;
    skinMatrix[2][3] = 0.0f;
    //skinMatrix = transpose(skinMatrix);
    outputVertex.Normal = mul(inputVertex.normal, skinMatrix);
    
    outputVertex.Diffuse = inputVertex.diffuse;
    outputVertex.TexCoord = inputVertex.uv;
    
    //格納用バッファに入れていく
    outPutVertexBuffer[index] = outputVertex;
    
}