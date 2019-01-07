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

// アンチャーテッド２トーンマッピング
static const float A = 0.15;
static const float B = 0.50;
static const float C = 0.10;
static const float D = 0.20;
static const float E = 0.02;
static const float F = 0.30;
static const float W = 11.2;

float3 Uncharted2Tonemap(float3 x) {
	return ((x*(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E / F;
}

float3 ApplyUncharted2Tonemap(float3 color) {
	float3 curr = Uncharted2Tonemap(color * g_Exposure);
	float3 white_scale = 1.0 / Uncharted2Tonemap(W);
	return curr * white_scale;
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	// ベースマップのピクセルを取得
	float4 Result = g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy);
	// トーンマッピングを行う
	Result.xyz = ApplyUncharted2Tonemap(Result.xyz);
	// ガンマ調整
	Result.xyz = pow(abs(Result.xyz), g_Gamma.xyz);
	// 最終カラーの出力
	output.Color0 = float4(Result.rgb, 1.0);
	return output;
}