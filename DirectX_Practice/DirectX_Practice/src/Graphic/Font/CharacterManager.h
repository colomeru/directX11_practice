#pragma once
#include "../SpritePtr.h"
#include <unordered_map>
#include <string>
#include <tchar.h>
#include "../RenderTexture.h"
#include "../../util/math/Vector2.h"

class Effect;

class CharacterManager
{
public:
	// �����\����
	struct Character
	{
		SpritePtr	pTexture;
		Vector2		uv;
	};

public:
	// �R���X�g���N�^
	CharacterManager();
	// �f�X�g���N�^
	~CharacterManager();

	// �t�H���g�f�[�^����
	void Create(const std::string& fileName, int fontSize);
	// �`��
	void Draw(Vector2 position, const std::string& text, const Effect& effect);
	// �����f�[�^�̓������A�g���X�e�N�X�`���̎擾
	RenderTexture GetRenderTexture() const;

private:
	// �����f�[�^����
	void CreateCharacter(TCHAR * c);
	// �����f�[�^����
	void CreateCharacter(const std::string& str);

public:

private:
	std::string		m_FontName;
	int				m_FontSize;
	RenderTexture	m_FontTexture;

	CComPtr<ID3D11Buffer>	m_pConstantBuffer;	// �萔�o�b�t�@

	std::unordered_map<std::string, Character>	m_FontMap;
	int		m_RowCount;
	int		m_ColCount;
};
