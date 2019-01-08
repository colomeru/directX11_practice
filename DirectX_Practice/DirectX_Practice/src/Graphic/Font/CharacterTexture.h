#pragma once
#include <string>
#include "../../DirectX/Texture.h"

class CharacterTexture
{
public:
	// コンストラクタ
	CharacterTexture();
	// デストラクタ
	~CharacterTexture();
	// 生成
	Texture* Create(const std::string& fontName, TCHAR* c, int fontsize);
	// 取得
	Texture* Get();

private:
	Texture		m_Texture;
};
