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

	// �����_�[�^�[�Q�b�g�p�̃e�N�X�`�����Q�Ƃ���
	float4 TextureDiffuseColor = g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord);
	// �O���[�X�P�[���ɕϊ�
	float intensity = dot(TextureDiffuseColor.rgb, float3(0.299, 0.587, 0.114));
	// �o�̓J���[�̐ݒ�
	output.Color0.rgb = intensity;
	output.Color0.a	  = 1.0;
	return output;
}