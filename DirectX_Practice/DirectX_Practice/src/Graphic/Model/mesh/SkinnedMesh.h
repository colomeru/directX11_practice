#pragma once
#include "../Mesh.h"
#include "../Animation.h"
#include "../Skeleton.h"
#include <atlbase.h>

//
class SkinnedMesh
{
public:
	//
	class Shader : public Mesh::Shader
	{
	public:
		// 仮想デストラクタ
		virtual ~Shader() = default;
		// ボーンの変換行列を設定
		virtual void BoneMatrices(int size, const Matrix matrices[]) = 0;
	};

public:
	// コンストラクタ
	SkinnedMesh(Mesh& mesh, Skeleton& skeleton, Animation& animation);
	// デストラクタ
	~SkinnedMesh();
	//
	void Calculate(const Matrix& world, float frameNo);
	//
	void Draw(SkinnedMesh::Shader& shader) const;

private:
	// ボーンの最大数
	static const unsigned int BONE_MAX = 256;
	// メッシュ
	const Mesh*				m_pMesh;
	// スケルトン
	const Skeleton*			m_pSkeleton;
	// アニメーション
	const Animation*		m_pAnimation;
	// ローカル変換行列
	Matrix					m_LocalMatrices[BONE_MAX];
	// ワールド変換行列
	Matrix					m_WorldMatrices[BONE_MAX];
	// スキニング変換行列
	Matrix					m_SkinnedMatrices[BONE_MAX];
	// ボーン情報送信用コンスタントバッファ
	CComPtr<ID3D11Buffer>	m_pConstantBuffer;
};
