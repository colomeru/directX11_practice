#include "ShadowMapShader.h"

ShadowMapShader::ShadowMapShader(Effect & effect) :
	m_Effect(effect)
{
}

void ShadowMapShader::Begin()
{
}

void ShadowMapShader::End()
{
}

void ShadowMapShader::Material(const Mesh::Material & material, ID3D11Buffer * constantBuffer, ID3D11SamplerState * sampler)
{
}
