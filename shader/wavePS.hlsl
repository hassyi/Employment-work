#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureEnv : register(t1);
SamplerState g_SamplerState : register(s0);

static float PI = 3.141592653589;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//視線ベクトル
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    //法線ベクトル
    float3 normal = normalize(In.Normal.xyz);
    
    //反射
    float3 refColor;
    {
        //反射ベクトル
        float3 refv = reflect(eyev, normal); //反射のベクトルはreflect()
        refv = normalize(refv);
    
        //環境マッピングテクスチャ座標(スフィアマップ)

        //float2 envTexCoord;
        //envTexCoord.x = refv.x * 0.3f + 0.5f;
        //envTexCoord.y = -refv.y * 0.3f + 0.5f;
	
        //環境マッピングテクスチャ座標(パノラマ)
        float2 envTexCoord;
        envTexCoord.x = atan(refv.x * refv.z) / (PI * 2);
        envTexCoord.y = acos(refv.y) / PI;
	
        refColor = g_TextureEnv.Sample(g_SamplerState, envTexCoord);
        
    }
    
    //屈折
    float3 refrColor;
    {
        //屈折ベクトル
        float3 refrv = refract(eyev.xyz, normal, 1.0f / 1.5f); //屈折ベクトルはrefract()
        refrv = normalize(refrv);
    
    
        //環境マッピングテクスチャ座標(パノラマ)
        float2 envTexCoord;
        envTexCoord.x = atan(refrv.x * refrv.z) / (PI * 2);
        envTexCoord.y = acos(refrv.y) / PI;

        //屈折マッピング
        refrColor = g_TextureEnv.Sample(g_SamplerState, envTexCoord);
    }
    
    float f0 = 0.04f;
    float d = saturate(dot(-eyev, normal));
    float fresnel = f0 + (1.0f - f0) * pow((1.0f - d), 5.0f);

    //outDiffuse.rgb = lerp(refrColor, refColor, fresnel);
    outDiffuse.rgb = refColor;
    outDiffuse.a = fresnel;

}

