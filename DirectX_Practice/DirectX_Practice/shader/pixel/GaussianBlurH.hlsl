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

	// テクスチャサイズの取得
	float2 size;
	g_BaseMapTexture.GetDimensions(size.x, size.y);
	// テクスチャオフセットを計算
	float2 tex_offset = 1.0 / size;
	// ガウシアンブラーの計算
	float4 result = 0.0;
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy) * g_GaussianWeights[0].x;
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy + float2(tex_offset.x * 1, 0.0)) * g_GaussianWeights[0].y;
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy + float2(tex_offset.x * 2, 0.0)) * g_GaussianWeights[0].z;
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy + float2(tex_offset.x * 3, 0.0)) * g_GaussianWeights[0].w;
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy + float2(tex_offset.x * 4, 0.0)) * g_GaussianWeights[1].x;
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy - float2(tex_offset.x * 1, 0.0)) * g_GaussianWeights[0].y;
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy - float2(tex_offset.x * 2, 0.0)) * g_GaussianWeights[0].z;
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy - float2(tex_offset.x * 3, 0.0)) * g_GaussianWeights[0].w;
	result += g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy - float2(tex_offset.x * 4, 0.0)) * g_GaussianWeights[1].y;
	output.Color0 = float4(result.rgb, 1.0f);
	return output;
}