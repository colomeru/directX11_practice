#pragma once
#include <d3d11.h>
#include <string>
#include "../../DirectX/Texture.h"

class FontTexture
{
public:
	// コンストラクタ
	FontTexture();
	// デストラクタ
	~FontTexture();
	// 生成
	Texture* Create(const std::string& fontName, TCHAR* c, int fontsize);

private:
	Texture		m_Texture;
};
