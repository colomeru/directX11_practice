#include "BloomCB.hlsli"

struct PS_INPUT
{
	float4 Position		: SV_POSITION;
	float2 Texcoord		: TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 Color0		: SV_TARGET;
};

SamplerState g_BaseMapSampler	: register(s0);
Texture2D	 g_BaseMapTexture	: register(t0);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	// �e�N�X�`���T�C�Y�̎擾
	float2 size;
	g_BaseMapTexture.GetDimensions(size.x, size.y);
	// �e�N�X�`���I�t�Z�b�g���v�Z
	float2 tex_offset = 1.0 / size;
	// ���͂S�s�N�Z���̕��ς����߂�
	float4 result = 0.0;
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy + float2(-0.5, 0.5) * tex_offset);
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy + float2(-0.5, -0.5) * tex_offset);
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy + float2(0.5, 0.5) * tex_offset);
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy + float2(0.5, -0.5) * tex_offset);
	result *= 0.25;
	// �s�N�Z���̋P�x�����߂�
	float luminance = dot(result.rgb, float3(0.33, 0.34, 0.33));
	// �w�肵���P�x�̈ȏ�̃s�N�Z�������c��
	luminance = max(0.0, luminance - g_BrightPassThreshold);
	result.rgb *= sign(luminance);
	output.Color0 = float4(result.rgb, 1.0f);
	return output;
}