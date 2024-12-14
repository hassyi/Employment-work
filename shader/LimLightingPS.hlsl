#include "common.hlsl"

Texture2D g_Texture : register(t0);         //テクスチャ0番
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //ピクセルの法線の正規化
    float3 normal = normalize(In.Normal);
    //光源計算
    float light = 0.5f - dot(normal.xyz, Light.Direction.xyz) * 0.5f;
    
    //テクスチャの色を取得
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    //光源計算の乗算する
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    //α値の処理
    outDiffuse.a *= In.Diffuse.a;
    
    //スペキュラー
    //視線ベクトル作成
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    //光の反射ベクトル
    float3 refv = reflect(Light.Direction.xyz, normal.xyz);
    refv = normalize(refv);
    //スペキュラーの計算
    float specular = -dot(eyev, refv);
    specular = saturate(specular);
    specular = pow(specular, 30.0f);        //スペキュラの調整
    
    //スペキュラーをディフューズに加算する
    outDiffuse.rgb += specular;
    
    //リムライティング処理
    //光の方向と視線ベクトルの考慮(逆光ほど明るい)
    float lit = 1.0f - max(0, dot(Light.Direction.xyz, eyev.xyz));
    //輪郭部分ほど明るくする
    float lim = 1.0f - max(0, dot(normal.xyz, -eyev));
    //litと limの明るさを合成
    lim *= lit;
    lim = pow(lim, 3);
    
    //リムライティングの明るさをディフューズに加算
    outDiffuse.rgb += lim;
    
    
}