#pragma once
#include "../Mesh.h"

class ShadowMapMesh : public Mesh
{
	// スキニングメッシュシェーダー
	class Shader : public Mesh::Shader {
	public:
		// 仮想デストラクタ
		virtual ~Shader() {};
	};

public:
	// コンストラクタ
	explicit ShadowMapMesh(Mesh& mesh);
	// 描画
	void Draw(ShadowMapMesh::Shader& shader) const;


private:
	// メッシュ
	const Mesh*			m_Mesh;
};
