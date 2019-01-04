#pragma once
#include "../ID.h"
#include "ShaderManager.h"

class Effect
{
public:
	// コンストラクタ
	Effect();
	// コンストラクタ
	Effect(VERTEX_SHADER_ID vsID, PIXEL_SHADER_ID psID);
	// コンストラクタ
	explicit Effect(VERTEX_SHADER_ID vsID);
	// コンストラクタ
	explicit Effect(PIXEL_SHADER_ID psID);
	// デストラクタ
	~Effect();
	// 読み込み
	void Load(VERTEX_SHADER_ID vsID, PIXEL_SHADER_ID psID);
	// シェーダー開始
	void Begin() const;
	// シェーダー終了
	void End() const;

private:
	VSPtr		m_VSShader;	// 頂点シェーダー
	PSPtr		m_PSShader;	// ピクセルシェーダー
};
