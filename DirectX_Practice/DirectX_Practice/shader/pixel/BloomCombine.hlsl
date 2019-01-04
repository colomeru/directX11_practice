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

SamplerState g_BaseSampler	: register(s0);		// ���摜�e�N�X�`��
Texture2D	 g_BaseTexture	: register(t0);		// ���摜�e�N�X�`��

SamplerState g_BloomSampler	: register(s1);		// �u���[���e�N�X�`��
Texture2D	 g_BloomTexture	: register(t1);		// �u���[���e�N�X�`��

// �ʓx��ύX����
float3 AdjustSaturation(float3 color, float saturation)
{
	// �O���[�X�P�[���ɕϊ�
	float Grey = dot(color, float3(0.3, 0.59, 0.11));
	// �ʓx�̒���
	return lerp(Grey, color, saturation);
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	// �x�[�X�J���[�̐F����
	float4 BaseColor = g_BaseTexture.Sample(g_BaseSampler, input.Texcoord.xy);
	BaseColor.rgb = AdjustSaturation(BaseColor.rgb, g_BaseSaturation).rgb * g_BaseIntensity;
	// �u���[���J���[�̐F����
	float4 BloomColor = g_BloomTexture.Sample(g_BloomSampler, input.Texcoord.xy);
	BloomColor.rgb = AdjustSaturation(BloomColor.rgb, g_BloomSaturation).rgb * g_BloomIntensity;
	// �x�[�X�J���[�ƃu���[���J���[�̍���
	float3 Result = BaseColor.rgb + BloomColor.rgb;
	// �ŏI�J���[�̏o��
	output.Color0 = float4(Result.rgb, 1.0);
	return output;
}