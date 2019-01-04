#pragma once
#include "../util/Singleton.h"
#include <string>
#include <memory>
#include "vertex/VertexShader.h"
#include "pixel/PixelShader.h"
#include <unordered_map>
#include "../ID.h"

// 頂点シェーダーのシェアードポインタ
using VSPtr = std::shared_ptr<VertexShader>;
// ピクセルシェーダークラスのシェアードポインタ
using PSPtr = std::shared_ptr<PixelShader>;

class ShaderManager : public Singleton<ShaderManager>
{
public:
	// Singleton でのインスタンス作成は許可
	friend class Singleton<ShaderManager>;

public:
	// 頂点シェーダの追加
	void Add(VERTEX_SHADER_ID id, const VSPtr shader);
	// ピクセルシェーダの追加
	void Add(PIXEL_SHADER_ID id, const PSPtr shader);
	// 頂点シェーダの取得
	VSPtr Get(const VERTEX_SHADER_ID id) const;
	// ピクセルシェーダの取得
	PSPtr Get(const PIXEL_SHADER_ID id) const;
	// クリア
	void Clear();

protected:
	// コンストラクタ
	ShaderManager();
	// デストラクタ
	virtual ~ShaderManager();

private:
	// 頂点シェーダマップ
	std::unordered_map<VERTEX_SHADER_ID, VSPtr>	m_VSUMap;
	// ピクセルシェーダマップ
	std::unordered_map<PIXEL_SHADER_ID, PSPtr>	m_PSUMap;
};
