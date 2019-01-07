#pragma once
#include <d3d11.h>
#include <string>
#include "../../DirectX/Texture.h"

class FontTexture
{
public:
	// �R���X�g���N�^
	FontTexture();
	// �f�X�g���N�^
	~FontTexture();
	// ����
	Texture* Create(const std::string& fontName, TCHAR* c, int fontsize);

private:
	Texture		m_Texture;
};
