struct VS_INPUT
{
	float3 Position		: POSITION;
	float3 Normal		: NORMAL0;
	float2 Texcoord		: TEXCOORD0;
	float4 BoneIndices	: TEXCOORD1;
	float4 BoneWeight	: TEXCOORD2;
	//float3 Tangent		: TEXCOORD3;
	//float3 Binormal		: TEXCOORD4;
};

struct VS_OUTPUT
{
	float4 Position		: SV_POSITION;
	float4 PositionW	: POSITION0;
	float4 NormalW		: NORMAL0;
	float2 Texcoord		: TEXCOORD0;
};

cbuffer ViewProjCB : register(b0)
{
	float4x4 View;			// ビュー変換行列
	float4x4 Projection;	// 透視射影変換行列
};

cbuffer WorldCB : register(b1)
{
	float4x4 World;			// ワールド変換行列
}

cbuffer ShadowCB : register(b2)
{
	float4x4 Shadow;
};

cbuffer BonesCB : register(b3)
{
	float4x4 Bones[256];	// ワールド変換行列
}


VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT	output;

	float4x4 matLocal = Bones[int(input.BoneIndices.x)] * input.BoneWeight.x
					  + Bones[int(input.BoneIndices.y)] * input.BoneWeight.y
					  + Bones[int(input.BoneIndices.z)] * input.BoneWeight.z
					  + Bones[int(input.BoneIndices.w)] * input.BoneWeight.w;
	//float4x4 matLocal = Bones[int(input.BoneIndices.x)] * input.BoneWeight.x
	//	+ Bones[int(input.BoneIndices.y)] * (1.0 - (input.BoneWeight.x + input.BoneWeight.z + input.BoneWeight.w))
	//	+ Bones[int(input.BoneIndices.z)] * input.BoneWeight.z
	//	+ Bones[int(input.BoneIndices.w)] * input.BoneWeight.w;

	//float4x4 matLocal = Bones[10];
	//float4x4 matLocal = float4x4(float4(1.0, 0.0, 0.0, 1.0), float4(0.0, 1.0, 0.0, 0.0), float4(0.0, 0.0, 1.0, 0.0), float4(0.0, 0.0, 0.0, 1.0));

	float4 local = mul(matLocal, float4(input.Position, 1.0));
	float4 world = mul(local, World);
	float4 view  = mul(world, View);
	float4 proj  = mul(view, Projection);
	output.Position = proj;



	output.PositionW = world;
	output.NormalW	 = mul(float4(input.Normal, 1.0f), World);
	output.Texcoord  = input.Texcoord;

	return output;
}