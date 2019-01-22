struct PS_INPUT
{
	float4 Position		: SV_POSITION;
	float4 PositionW	: POSITION0;
	float4 NormalW		: NORMAL0;
	float2 Texcoord		: TEXCOORD0;
};

struct Light
{
	float4 pos;
	float4 diffuse;
	float4 specular;
	float4 attenuate;
};

cbuffer Material	: register(b0)
{
	float4 u_ambient;
	float4 u_diffuse;
	float4 u_specular;
};

cbuffer ConstantBuffer	: register(b1)
{
	float4		eyePos;
	float4		ambient;
	Light		pntLight;
};

cbuffer ShadowBias : register(b3)
{
	float  bias;
	float  slopeScaledBias;
	float  depthBiasClamp;
};

SamplerState	g_BaseMapSampler	: register(s0);
Texture2D		g_BaseMapTexture	: register(t0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// 法線ベクトルを正規化
	float3 N = normalize(input.NormalW.xyz);
	///*
	// 視点方向ベクトルを正規化
	float3 V = normalize(-input.PositionW.xyz);
	// ライト方向ベクトルを正規化
	float3 L = pntLight.pos.xyz - input.PositionW.xyz;
	//float3 L = mul(pntLight.pos.xyz, input.View);
	L = normalize(L - eyePos.xyz);
	// ２等分ベクトルを求める
	float3 H = normalize(L + V);
	// テクスチャーカラーの取得
	float4 baseColor = g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy);

	float  diffuse = max(dot(N, L), 0.0f);
	float  specular = pow(max(dot(N, H), 0.0f), u_specular.w);

	// 環境光を求める
	float3 AmbientColor = u_ambient.rgb  * ambient.rgb * baseColor.rgb;
	// 拡散反射光を求める
	float3 DiffuseColor = u_diffuse.rgb  * pntLight.diffuse.rgb  * baseColor.rgb * diffuse;
	// 鏡面反射光を求める
	float3 SpecularColor = u_specular.rgb * pntLight.specular.rgb * specular;
	// 自己照明を求める
	float3 EmissiveColor = baseColor.rgb * u_ambient.rgb;

	float4 OutputColor;
	OutputColor.rgb = saturate(AmbientColor + DiffuseColor + SpecularColor);
	OutputColor.a = u_diffuse.a;
	return OutputColor;
}