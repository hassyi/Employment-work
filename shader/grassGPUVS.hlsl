
#include "common.hlsl"

StructuredBuffer<float3> Position : register(t2);

//乱数
float random(float3 input)
{
    return frac(sin(dot(input.xz, float2(12.9898, 78.233))) * 43758.5453123);
}


void main(in VS_IN In, out PS_IN Out)
{

	//matrix wvp;
	//wvp = mul(World, View);
	//wvp = mul(wvp, Projection);
	
    Out.Position = mul(In.Position, World);				//座標変換をワールドにする
	
    //In.Position.x += In.InstanceId * 2.0f;
    Out.Position.xyz += Position[In.InstanceId];
    //Out.Position.x += random(Position[In.InstanceId] * 100.0f - 50.0f);
    //Out.Position.z += random(Position[In.InstanceId] * 100.0f - 50.0f);

	Out.Position = mul(Out.Position, View);
	Out.Position = mul(Out.Position, Projection);
	
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse * Material.Diffuse;

}

