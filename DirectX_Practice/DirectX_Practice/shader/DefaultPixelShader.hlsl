struct PS_INPUT
{
	float4 Position	: SV_POSITION;
	float4 Texcoord	: TEXCOORD0;
};

SamplerState	g_BaseMapSampler	: register(s0);
Texture2D		g_BaseMapTexture	: register(t0);

float4 main(PS_INPUT input) : SV_TARGET
{
	return g_BaseMapTexture.Sample(g_BaseMapSampler, input.Texcoord.xy);
}