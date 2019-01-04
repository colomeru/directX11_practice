#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include "../util/Singleton.h"
#include <string>
#include <unordered_map>
#include "FontManager.h"
#include "../Shader/Effect.h"

struct Vector2;

class DrawFont : public Singleton<DrawFont>
{
public:
	//
	friend class Singleton<DrawFont>;

public:
	// �t�H���g�ǂݍ���
	void Load(const std::string& fileName, int fontSize);
	// �`��
	void Draw(Vector2 position, const std::string& text);

	// �ϒ�������string�ɂ��ĕԂ�
	std::string format(const char* format, ...);

private:
	// �R���X�g���N�^
	DrawFont();
	// �f�X�g���N�^
	virtual ~DrawFont();

public:
	FontManager					m_DebugFont;
	Effect						m_Effect;
};
