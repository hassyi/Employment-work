#include "common.hlsl"

Texture2D g_Texture : register(t0);         //�e�N�X�`��0��
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //�s�N�Z���̖@���̐��K��
    float4 normal = normalize(In.Normal);
    //�����v�Z
    float light = 0.5f - dot(normal.xyz, Light.Direction.xyz) * 0.5f;
    
    //�e�N�X�`���̐F���擾
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    //�����v�Z�̏�Z����
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    //���l�̏���
    outDiffuse.a *= In.Diffuse.a;
    
    //�X�y�L�����[
    //�����x�N�g���쐬
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    //���̔��˃x�N�g��
    float3 refv = reflect(Light.Direction.xyz, normal.xyz);
    refv = normalize(refv);
    //�X�y�L�����[�̌v�Z
    float specular = -dot(eyev, refv);
    specular = saturate(specular);
    specular = pow(specular, 30.0f);        //�X�y�L�����̒���
    
    //�X�y�L�����[���f�B�t���[�Y�ɉ��Z����
    outDiffuse.rgb += specular;
    
    //�������C�e�B���O����
    //���̕����Ǝ����x�N�g���̍l��(�t���قǖ��邢)
    float lit = 1.0f - max(0, dot(Light.Direction.xyz, eyev.xyz));
    //�֊s�����قǖ��邭����
    float lim = 1.0f - max(0, dot(normal.xyz, -eyev));
    //lit�� lim�̖��邳������
    lim *= lit;
    lim = pow(lim, 3);
    
    //�������C�e�B���O�̖��邳���f�B�t���[�Y�ɉ��Z
    outDiffuse.rgb += lim;
    
    
}