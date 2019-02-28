#include "FontManager.h"
#include "../../DirectX/DirectX11.h"
#include "../../util/math/Vector2.h"
#include "../../Shader/ShaderManager.h"
#include <stdarg.h>

FontManager::FontManager() :
	m_DebugFontManager(),
	m_Effect(VERTEX_SHADER_ID::FONT_SHADER, PIXEL_SHADER_ID::TEXTURE_SHADER)
{
	//m_DebugFont.Create("Ricty Diminished", 12);
	m_DebugFontManager.Create("APJapanesefontF", 12);
}

FontManager::~FontManager()
{
}

void FontManager::Load(const std::string & fileName, int fontSize)
{
}

void FontManager::DebugDraw(Vector2 position, const std::string& text)
{
#ifdef DEBUG
	DirectX11::GetInstance()->SetAlphaBlend(true, false);
	m_DebugFontManager.Draw(position, text, m_Effect);
	DirectX11::GetInstance()->SetAlphaBlend(false, false);
#endif // DEBUG
}

CharacterManager FontManager::GetDebugFont() const
{
	return m_DebugFontManager;
}
