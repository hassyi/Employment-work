
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float3 normal = normalize(In.Normal);
    float light = saturate(dot(normal.xyz, Light.Direction.xyz));
    
    if(light >= 0.7f)
    {
        light = 1.0f;
    }
    else if(light >= 0.5f)
    {
        light = 0.8f;
    }
    else
    {
        light = 0.4f;
    }

    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a = In.Diffuse.a;

}
