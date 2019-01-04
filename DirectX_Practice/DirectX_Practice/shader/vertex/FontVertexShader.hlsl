struct VS_INPUT
{
	float3 Position	: POSITION;
	float2 Texcoord	: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position		: SV_POSITION;
	float2 Texcoord		: TEXCOORD0;
};

cbuffer CBUV :register(b0)
{
	float2 uv_start;
	float2 uv_size;
};

cbuffer CBuffTexture : register(b1)
{
	float4x4 World;
	float4x4 Projection;	// ìßéãéÀâeïœä∑çsóÒ
}

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 world = mul(float4(input.Position, 1.0f), World);
	float4 proj = mul(world, Projection);
	output.Position = proj;

	output.Texcoord = input.Texcoord * uv_size + uv_start;

	return output;
}