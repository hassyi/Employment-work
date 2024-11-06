#include "common.hlsl"

Texture2D	g_Texture : register(t0);			//0番テクスチャの情報
SamplerState g_SamplerState : register(s0);		//サンプラー0番


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//テクスチャの色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;		//テクスチャの色に頂点の明るさを乗算

	//outDiffuse = In.Diffuse;		//ピクセルの明るさで直接描画



}

