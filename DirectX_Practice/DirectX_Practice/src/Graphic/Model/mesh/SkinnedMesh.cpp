#include "SkinnedMesh.h"
#include "../../../DirectX/DirectX11.h"
#include "../../../ConstantBuffer/ConstantBuffer.h"

SkinnedMesh::SkinnedMesh(Mesh& mesh, Skeleton& skeleton, Animation& animation):
	m_pMesh(&mesh),
	m_pSkeleton(&skeleton),
	m_pAnimation(&animation)
{
	DirectX11::GetInstance()->CreateBuffer(&m_pConstantBuffer, nullptr, sizeof(BonesCB), D3D11_BIND_CONSTANT_BUFFER);
}

SkinnedMesh::~SkinnedMesh()
{
}

void SkinnedMesh::Calculate(const Matrix & world, float frameNo)
{
	// アニメーション変換行列の計算
	m_pSkeleton->CalculateLocalMatrices(*m_pAnimation, frameNo, m_LocalMatrices);
	// ワールド変換行列の作成
	m_pSkeleton->CalculateWorldMatrices(world, m_LocalMatrices, m_WorldMatrices);
	// スキニング変換行列の作成
	m_pSkeleton->CalculateSkinnedMatrices(m_WorldMatrices, m_SkinnedMatrices);
}

void SkinnedMesh::Draw(SkinnedMesh::Shader & shader) const
{
	shader.BoneMatrices(m_pSkeleton->Size(), m_SkinnedMatrices);
	
	// 定数バッファ情報の送信
	BonesCB cb;
	int length = sizeof(cb.bones) / sizeof(*cb.bones);
	for (int i = 0; i < length || i < m_pSkeleton->Size(); ++i){
		cb.bones[i] = m_SkinnedMatrices[i];
	}

	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	DirectX11::GetInstance()->GetContext()->VSSetConstantBuffers(3, 1, &m_pConstantBuffer.p);

	m_pMesh->Draw(shader);
}
