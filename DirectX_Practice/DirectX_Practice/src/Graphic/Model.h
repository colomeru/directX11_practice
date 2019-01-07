#pragma once
#include "Model/Mesh.h"
#include "Model/Skeleton.h"

struct Matrix;

class Model
{
public:
	// コンストラクタ
	Model();
	// デストラクタ
	~Model();
	// 描画
	void Draw(Mesh::Shader& shader, const Matrix& world);
	// 影用描画
	void DrawForShadow(Mesh::Shader& shader, const Matrix& world);

	// アルファブレンドの設定
	void SetAlphaBlend(bool alphaEnable, bool alphaToCoverageEnable);

	// コピー禁止
	Model(const Model&) = delete;
	Model& operator = (const Model&) = delete;

public:
	Mesh		mesh;		// メッシュ
	Skeleton	skeleton;	// スケルトン

private:
	/// <bool alphaEnable, bool alphaToCoverageEnable>
	typedef std::pair<bool, bool>	AlphaBlend;
	AlphaBlend				m_AlphaBlend;	// アルファブレンド
	CComPtr<ID3D11Buffer>	m_pConstantBuffer;	// 定数バッファ
};
