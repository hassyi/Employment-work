
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;
    Out.Diffuse.r = 1.0f;
    Out.Diffuse.gb = 0.0f;
    Out.Diffuse.a = 1.0f;
}

