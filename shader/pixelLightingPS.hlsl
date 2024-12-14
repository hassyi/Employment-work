#include "common.hlsl"

Texture2D	g_Texture : register(t0);			//0番テクスチャの情報
SamplerState g_SamplerState : register(s0);		//サンプラー0番


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float3 normal = normalize(In.Normal);
	float light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);

	//テクスチャのピクセル色の取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb * light;		//明るさを乗算
	outDiffuse.a *= In.Diffuse.a;					//αに明るさは関係ないので別計算

	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.rgb * light;
	eyev = normalize(eyev);

	//光の反射ベクトル
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	refv = normalize(refv);			//正規化する

	//スペキュラの計算
	float specular = -dot(eyev, refv);		//鏡面反射の計算
	specular = saturate(specular);			//値をサチュレート	
	specular = pow(specular, 30);			//ここでは30乗にしてみる

	outDiffuse.rgb += specular;				//スペキュラ値をディフューズして足しこむ

}

