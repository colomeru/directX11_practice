#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include <string>
#include "../../util/Singleton.h"
#include <unordered_map>
#include "FontManager.h"
#include "../../Shader/Effect.h"
#include <iostream>

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

private:
	// コンストラクタ
	DrawFont();
	// デストラクタ
	virtual ~DrawFont();

public:
	FontManager					m_DebugFont;
	Effect						m_Effect;
};
