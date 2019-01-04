struct VS_INPUT
{
	float3 Position	: POSITION;
};

struct VS_OUTPUT
{
	float4 Position	: SV_POSITION0;
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

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 world = mul(float4(input.Position, 1.0f), World);
	float4 view  = mul(world, View);
	float4 proj  = mul(view, Projection);

	output.Position = proj;

	return output;
}