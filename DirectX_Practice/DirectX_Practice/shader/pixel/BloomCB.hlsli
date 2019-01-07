#ifndef BLOOM_CB_HLSLI_
#define BLOOM_CB_HLSLI_

// ブルームエフェクト用定数バッファ
cbuffer BloomCB : register(b0)
{
	// 輝度の閾値
	float g_BrightPassThreshold;
	// 元画像のテクスチャの彩度
	float g_BaseSaturation;
	// 元画像のテクスチャの輝度
	float g_BaseIntensity;
	// ブルームテクスチャの彩度
	float g_BloomSaturation;
	// ブルームテクスチャの輝度
	float g_BloomIntensity;
	// ガウシアンブラーのウェイト値
	float4 g_GaussianWeights[2];
	// ガンマ値
	float4 g_Gamma;
	// 露出調整
	float  g_Exposure;
}

#endif