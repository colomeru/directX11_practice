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

// ライト定数バッファ
struct ConstantLight
{
	Vector3	position;
	float	padding[1];
	Color	diffuse;
	Color	specular;
	Color	attenuate;
};

// マテリアル定数バッファ
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
	// 輝度の閾値
	float g_BrightPassThreshold;
	// 元画像のテクスチャの彩度
	float g_BaseSaturation{ 1.0f };
	// 元画像のテクスチャの輝度
	float g_BaseIntensity{ 1.0f };
	// ブルームテクスチャの彩度
	float g_BloomSaturation{ 1.0f };
	// ブルームテクスチャの輝度
	float g_BloomIntensity{ 1.0 };
	float padding1[3]{ 0.0 };
	// ガウシアンブラーのウェイト値
	float g_GaussianWeights[5]{ 0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f };
	float padding2[3]{ 0.0f };
	// ガンマ値
	Vector3 g_Gamma{ 1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f };
	float padding3[1]{ 0.0f };
	// 露出調整
	float g_Exposure{ 4.0f };
	float padding4[3]{ 0.0f };
};

struct ShadowCB
{
	Matrix	g_Shadow;
};

struct ShadowBiasCB
{
	// 固定バイアス
	float	g_Bias;
	// 深度傾斜
	float	g_SlopeScaledBias;
	// バイアスクランプ値
	float	g_DepthBiasClamp;
	float	padding[1]{ 0.0f };
};