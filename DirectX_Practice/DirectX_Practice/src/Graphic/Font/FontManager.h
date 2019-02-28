#pragma once
#include "../../util/Singleton.h"
#include <string>
#include "CharacterManager.h"
#include "../../Shader/Effect.h"

struct Vector2;

class FontManager : public Singleton<FontManager>
{
public:
	//
	friend class Singleton<FontManager>;

public:
	// �t�H���g�ǂݍ���
	void Load(const std::string& fileName, int fontSize);
	// �`��
	void DebugDraw(Vector2 position, const std::string& text);
	
	// Debug�p�t�H���g�̎擾
	CharacterManager GetDebugFont() const;

private:
	// �R���X�g���N�^
	FontManager();
	// �f�X�g���N�^
	virtual ~FontManager();

private:
	CharacterManager	m_DebugFontManager;
	Effect				m_Effect;
};
