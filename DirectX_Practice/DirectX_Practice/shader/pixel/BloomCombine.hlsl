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

SamplerState g_BaseSampler	: register(s0);		// 元画像テクスチャ
Texture2D	 g_BaseTexture	: register(t0);		// 元画像テクスチャ

SamplerState g_BloomSampler	: register(s1);		// ブルームテクスチャ
Texture2D	 g_BloomTexture	: register(t1);		// ブルームテクスチャ

// 彩度を変更する
float3 AdjustSaturation(float3 color, float saturation)
{
	// グレースケールに変換
	float Grey = dot(color, float3(0.3, 0.59, 0.11));
	// 彩度の調整
	return lerp(Grey, color, saturation);
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	// ベースカラーの色調整
	float4 BaseColor = g_BaseTexture.Sample(g_BaseSampler, input.Texcoord.xy);
	BaseColor.rgb = AdjustSaturation(BaseColor.rgb, g_BaseSaturation).rgb * g_BaseIntensity;
	// ブルームカラーの色調整
	float4 BloomColor = g_BloomTexture.Sample(g_BloomSampler, input.Texcoord.xy);
	BloomColor.rgb = AdjustSaturation(BloomColor.rgb, g_BloomSaturation).rgb * g_BloomIntensity;
	// ベースカラーとブルームカラーの合成
	float3 Result = BaseColor.rgb + BloomColor.rgb;
	// 最終カラーの出力
	output.Color0 = float4(Result.rgb, 1.0);
	return output;
}