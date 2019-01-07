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

	// ƒuƒŒƒ“ƒh
	DirectX11::GetInstance()->SetAlphaBlend(true, false);
	m_DebugFont.Draw(position, text);
	DirectX11::GetInstance()->SetAlphaBlend(false, false);

	m_Effect.End();
}
