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
	// フォント読み込み
	void Load(const std::string& fileName, int fontSize);
	// 描画
	void Draw(Vector2 position, const std::string& text);

private:
	// コンストラクタ
	FontManager();
	// デストラクタ
	virtual ~FontManager();

private:
	CharacterManager	m_DebugFontManager;
	Effect				m_Effect;
};
