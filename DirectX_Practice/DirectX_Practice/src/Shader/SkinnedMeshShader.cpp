#include "SkinnedMeshShader.h"

SkinnedMeshShader::SkinnedMeshShader(Effect & effect) :
	MeshShader(effect)
{
}

void SkinnedMeshShader::Begin()
{
	MeshShader::Begin();
}

void SkinnedMeshShader::End()
{
	MeshShader::End();
}

void SkinnedMeshShader::Material(const Mesh::Material & material, ID3D11Buffer * constantBuffer, ID3D11SamplerState * sampler)
{
	MeshShader::m_Material		  = material;
	MeshShader::m_pConstantBuffer = constantBuffer;
	MeshShader::m_pSampler		  = sampler;
}

void SkinnedMeshShader::BoneMatrices(int size, const Matrix matrices[])
{
	m_NumBones = size;
	std::copy(&matrices[0], &matrices[size], m_BoneMatrices);
}
