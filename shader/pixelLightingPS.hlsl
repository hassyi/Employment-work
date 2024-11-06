#include "common.hlsl"

Texture2D	g_Texture : register(t0);			//0�ԃe�N�X�`���̏��
SamplerState g_SamplerState : register(s0);		//�T���v���[0��


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal);
	float light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);

	//�e�N�X�`���̃s�N�Z���F�̎擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb * light;		//���邳����Z
	outDiffuse.a *= In.Diffuse.a;					//���ɖ��邳�͊֌W�Ȃ��̂ŕʌv�Z

	//�J��������s�N�Z���֌������x�N�g��
	float3 eyev = In.WorldPosition.rgb * light;
	eyev = normalize(eyev);

	//���̔��˃x�N�g��
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	refv = normalize(refv);			//���K������

	//�X�y�L�����̌v�Z
	float specular = -dot(eyev, refv);		//���ʔ��˂̌v�Z
	specular = saturate(specular);			//�l���T�`�����[�g	
	specular = pow(specular, 30);			//�����ł�30��ɂ��Ă݂�

	outDiffuse.rgb += specular;				//�X�y�L�����l���f�B�t���[�Y���đ�������

}

