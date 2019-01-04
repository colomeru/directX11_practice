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
	// フォント読み込み
	void Load(const std::string& fileName, int fontSize);
	// 描画
	void Draw(Vector2 position, const std::string& text);

	// 可変長引数をstringにして返す
	std::string format(const char* format, ...);

private:
	// コンストラクタ
	DrawFont();
	// デストラクタ
	virtual ~DrawFont();

public:
	FontManager					m_DebugFont;
	Effect						m_Effect;
};
