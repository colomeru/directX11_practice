#ifndef BLOOM_CB_HLSLI_
#define BLOOM_CB_HLSLI_

// �u���[���G�t�F�N�g�p�萔�o�b�t�@
cbuffer BloomCB : register(b0)
{
	// �P�x��臒l
	float g_BrightPassThreshold;
	// ���摜�̃e�N�X�`���̍ʓx
	float g_BaseSaturation;
	// ���摜�̃e�N�X�`���̋P�x
	float g_BaseIntensity;
	// �u���[���e�N�X�`���̍ʓx
	float g_BloomSaturation;
	// �u���[���e�N�X�`���̋P�x
	float g_BloomIntensity;
	// �K�E�V�A���u���[�̃E�F�C�g�l
	float4 g_GaussianWeights[2];
	// �K���}�l
	float4 g_Gamma;
	// �I�o����
	float  g_Exposure;
}

#endif