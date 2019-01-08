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
	// 文字構造体
	struct Character
	{
		SpritePtr	pTexture;
		Vector2		uv;
	};

public:
	// コンストラクタ
	CharacterManager();
	// デストラクタ
	~CharacterManager();

	// フォントデータ生成
	void Create(const std::string& fileName, int fontSize);
	// 描画
	void Draw(Vector2 position, const std::string& text, const Effect& effect);

private:
	// 文字データ生成
	void CreateCharacter(TCHAR * c);
	// 文字データ生成
	void CreateCharacter(const std::string& str);

public:
	RenderTexture	m_FontTexture;

private:
	std::string		m_FontName;
	int				m_FontSize;

	CComPtr<ID3D11Buffer>	m_pConstantBuffer;	// 定数バッファ

	std::unordered_map<std::string, Character>	m_FontMap;
	int		m_RowCount;
	int		m_ColCount;
};
