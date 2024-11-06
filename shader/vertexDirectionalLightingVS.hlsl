

#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	//頂点の変換
	matrix wvp;
	wvp = mul(World, View);					//wvp = World行列*View行列
	wvp = mul(wvp, Projection);				//wvp = wvp * Projection
	Out.Position = mul(In.Position, wvp);	//頂点座標*wvpの結果を出力

	//法線をワールド行列で回転させる(頂点と同じ回転をかける)
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);		//wは0としてコピーする
	worldNormal = mul(normal, World);			//コピーした法線*World行列
	worldNormal = normalize(worldNormal);		//法線を正規化する
	Out.Normal = worldNormal;					//回転後の法線を出力	In.Normalではない

	//光源計算	光ベクトルと法線ベクトルの内積
	float	light = -dot(Light.Direction.xyz, worldNormal.xyz);
	//float	light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);		//ハーフランバートでの明るさの計算
	light = saturate(light);		//lightの値を0.0-1.0で飽和化する

	//頂点のディフューズを出力
	Out.Diffuse.rgb = light;		//明るさの値を頂点色として出力
	Out.Diffuse.a = In.Diffuse.a;	//αは頂点の情報をそのまま出力

	//テクスチャ座標を出力
	Out.TexCoord = In.TexCoord;
}