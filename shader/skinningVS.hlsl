#include "common.hlsl"

StructuredBuffer<SKVS_IN> vertexBuffer : register(t0);        //スキニング処理が施された頂点バッファ

struct CSVS_IN
{
    uint vertexID : SV_VertexID;        //ユニークな頂点idが自動に格納される
};

void main(in CSVS_IN In, out PS_IN Out)
{
    SKVS_IN vtx = vertexBuffer[In.vertexID];
    
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    Out.Position = mul(vtx.Position, wvp);
    Out.TexCoord = vtx.TexCoord;
    Out.Diffuse = vtx.Diffuse * Material.Diffuse;
    //Out.Diffuse.a = 1.0f;
    
}