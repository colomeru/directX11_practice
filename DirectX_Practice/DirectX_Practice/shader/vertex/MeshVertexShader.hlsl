struct VS_INPUT
{
	float3 Position	: POSITION;
	float3 Normal	: NORMAL0;
	float2 Texcoord	: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position		: SV_POSITION;
	float4 PositionW	: POSITION0;
	float4 NormalW		: NORMAL0;
	float2 Texcoord		: TEXCOORD0;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4 View;			// ビュー変換行列
	float4x4 Projection;	// 透視射影変換行列
};

cbuffer CBuffObject : register(b1)
{
	float4x4 World;			// ワールド変換行列
}

cbuffer ShadowCB : register(b2)
{
	float4x4 Shadow;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT	output;

	float4 world = mul(float4(input.Position, 1.0f), World);
	float4 view  = mul(world, View);
	float4 proj  = mul(view, Projection);
	output.Position = proj;

	output.PositionW = world;
	output.NormalW	 = mul(float4(input.Normal, 1.0f), World);
	output.Texcoord  = input.Texcoord;
	return output;
}