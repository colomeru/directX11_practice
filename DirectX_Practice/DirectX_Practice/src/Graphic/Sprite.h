#pragma once
#include "../DirectX/ShaderResource.h"
#include <string>

class Texture;

class Sprite
{
public:
	// コンストラクタ
	Sprite();
	// デストラクタ
	~Sprite();
	// 描画
	void Begin() const;
	// 終了
	void End();
	// テクスチャから生成
	bool Create(const std::string& filePath);
	// テクスチャから生成
	bool Create(const Texture* pTexture);

	// 横幅取得
	unsigned int GetWidth() const;
	// 縦幅取得
	unsigned int GetHeight() const;
	// シェーダーリソース取得
	ShaderResource GetShaderResource() const;

private:
	unsigned int						m_Width;	// 横幅
	unsigned int						m_Height;	// 縦幅
	ShaderResource						m_Res;		// リソース
};