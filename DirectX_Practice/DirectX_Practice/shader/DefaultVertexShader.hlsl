struct VS_INPUT
{
	float3 Position	: POSITION;
	float3 Normal	: NORMAL0;
	float2 Texcoord	: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position	: SV_POSITION;
	float2 Texcoord	: TEXCOORD0;
};

cbuffer ConstantBuffer	: register(b0)
{
	float4x4 View;			// ビュー変換行列
	float4x4 Projection;	// 透視射影変換行列
	float4	 Light;
	float4	 Eye;
};

cbuffer CBufferObject	: register(b1)
{
	float4x4 World;			// ワールド変換行列
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT	output;

	output.Position = mul(float4(input.Position, 1.0f), World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Texcoord = input.Texcoord;

	return output;
}