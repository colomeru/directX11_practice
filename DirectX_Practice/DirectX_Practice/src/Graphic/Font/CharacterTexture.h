#pragma once
#include <string>
#include "../../DirectX/Texture.h"

class CharacterTexture
{
public:
	// �R���X�g���N�^
	CharacterTexture();
	// �f�X�g���N�^
	~CharacterTexture();
	// ����
	Texture* Create(const std::string& fontName, TCHAR* c, int fontsize);
	// �擾
	Texture* Get();

private:
	Texture		m_Texture;
};
