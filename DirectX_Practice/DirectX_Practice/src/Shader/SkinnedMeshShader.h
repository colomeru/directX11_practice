#pragma once
#include "../Graphic/Model/mesh/SkinnedMesh.h"
#include "MeshShader.h"

//
class SkinnedMeshShader : public MeshShader, public SkinnedMesh::Shader
{
public:
	// コンストラクタ
	explicit SkinnedMeshShader(Effect& effect);
	// シェーダー開始
	virtual void Begin() override;
	// シェーダー終了
	virtual void End() override;
	// マテリアルの設定
	virtual void Material(const Mesh::Material& material, ID3D11Buffer* constantBuffer, ID3D11SamplerState* sampler) override;
	// ボーンの変換行列を設定
	virtual void BoneMatrices(int size, const Matrix matrices[]) override;

private:
	// ボーンの最大数
	static const unsigned int BONE_MAX = 256;
	// ボーン数
	int			m_NumBones;
	// ボーンの変換行列
	Matrix		m_BoneMatrices[BONE_MAX];
};
