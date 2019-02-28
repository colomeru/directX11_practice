#pragma once

#include "../util/math/Vector2.h"
#include "../util/math/Vector3.h"
#include "../util/math/Matrix.h"
#include "../util/math/Color.h"

//
struct WorldCB
{
	Matrix	world;
};

struct ViewProjCB
{
	Matrix	view;
	Matrix	projection;
};

struct WorldProjCB
{
	Matrix	world;
	Matrix	projection;
};

struct BonesCB
{
	Matrix bones[256];
};

struct UV_CB
{
	Vector2	uv_start;
	Vector2	uv_size;
};

// ���C�g�萔�o�b�t�@
struct ConstantLight
{
	Vector3	position;
	float	padding[1];
	Color	diffuse;
	Color	specular;
	Color	attenuate;
};

// �}�e���A���萔�o�b�t�@
struct MaterialCB
{
	Color	ambient;
	Color	diffuse;
	Color	specular;
};

//
struct LightCB
{
	Vector3				eyePosition;
	float				padding[1];
	Color				ambient;
	ConstantLight		pntLight;
};


struct BloomCB
{
	// �P�x��臒l
	float g_BrightPassThreshold;
	// ���摜�̃e�N�X�`���̍ʓx
	float g_BaseSaturation{ 1.0f };
	// ���摜�̃e�N�X�`���̋P�x
	float g_BaseIntensity{ 1.0f };
	// �u���[���e�N�X�`���̍ʓx
	float g_BloomSaturation{ 1.0f };
	// �u���[���e�N�X�`���̋P�x
	float g_BloomIntensity{ 1.0 };
	float padding1[3]{ 0.0 };
	// �K�E�V�A���u���[�̃E�F�C�g�l
	float g_GaussianWeights[5]{ 0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f };
	float padding2[3]{ 0.0f };
	// �K���}�l
	Vector3 g_Gamma{ 1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f };
	float padding3[1]{ 0.0f };
	// �I�o����
	float g_Exposure{ 4.0f };
	float padding4[3]{ 0.0f };
};

struct ShadowCB
{
	Matrix	g_Shadow;
};

struct ShadowBiasCB
{
	// �Œ�o�C�A�X
	float	g_Bias;
	// �[�x�X��
	float	g_SlopeScaledBias;
	// �o�C�A�X�N�����v�l
	float	g_DepthBiasClamp;
	float	padding[1]{ 0.0f };
};