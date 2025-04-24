


cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}




struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	bool TextureEnable;
	float2 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}




struct LIGHT
{
	bool Enable;
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
	
    float4 SkyColor; //天球色
    float4 GroundColor; //地面色
    float4 GroundNormal; //地面の法線

    float4 Position; //光源の座標
    float4 PointLightParam; //光の範囲

    matrix ViewMatrix;
    matrix ProjectonMatrix;
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}





struct VS_IN
{
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
    uint InstanceId		: SV_InstanceID; //描画する回数を取得
};


struct PS_IN
{
	float4 Position		: SV_POSITION;
    float4 WorldPosition : POSITION0; //ピクセルの空間での座標
    float3 Normal : NORMAL0; //ピクセルの法線
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
    float4 LightPosition : POSITION1;
};

cbuffer CameraBuffer : register(b5)
{
    float4 CameraPosition; //カメラの座標
}
