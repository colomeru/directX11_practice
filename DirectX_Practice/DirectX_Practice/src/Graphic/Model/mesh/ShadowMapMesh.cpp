#include "ShadowMapMesh.h"

ShadowMapMesh::ShadowMapMesh(Mesh & mesh) :
	m_Mesh(&mesh)
{
}

void ShadowMapMesh::Draw(ShadowMapMesh::Shader & shader) const
{
	m_Mesh->Draw(shader);
}
