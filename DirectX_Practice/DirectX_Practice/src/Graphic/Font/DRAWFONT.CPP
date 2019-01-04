#include "DrawFont.h"
#include "../../DirectX/DirectX11.h"
#include "../../util/math/Vector2.h"
#include "../../util/math/Vector3.h"
#include "../../Shader/ShaderManager.h"
#include <stdarg.h>

DrawFont::DrawFont() :
	m_DebugFont(),
	m_Effect(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::TEXTURE_SHADER)
{
	//m_DebugFont.Create("Ricty Diminished", 12);
	m_DebugFont.Create("APJapanesefontF", 12);
}

DrawFont::~DrawFont()
{
}

void DrawFont::Load(const std::string & fileName, int fontSize)
{
}

void DrawFont::Draw(Vector2 position, const std::string& text)
{
	m_Effect.Begin();

	// ブレンド
	DirectX11::GetInstance()->SetAlphaBlend(true, false);
	m_DebugFont.Draw(position, text);
	DirectX11::GetInstance()->SetAlphaBlend(false, false);

	m_Effect.End();
}

std::string DrawFont::format(const char * format, ...)
throw(const char *)
{
	va_list v;				// 可変長引数型
	int num = 0;			// バイト数
	int res = 0;			// 返り値
	std::string ret;		// 返り値

	va_start(v, format);	// 可変長引数操作開始

	num = _vscprintf(format, v) + 1;				// 書式展開後のバイト数取得
	char* buf = new char[num];						// バッファメモリアロケーション

	res = _vsnprintf_s(buf, num, num, format, v);	// バッファに書式展開後の文字列を設定
	if (res < 0)
	{
		va_end(v);				// 可変長引数操作終了
		delete[] buf;			// バッファ解放
		throw "_vsnprintf_s error";
	}

	ret = buf;				// stringに変換

	va_end(v);				// 可変長引数操作終了
	delete[] buf;			// バッファ解放

	return ret;
}
