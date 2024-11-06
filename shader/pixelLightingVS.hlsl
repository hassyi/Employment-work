
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

	Out.Diffuse = In.Diffuse;				//頂点職の出力
	Out.TexCoord = In.TexCoord;				//テクスチャ座標の出力

	Out.WorldPosition = mul(In.Position, World);

}