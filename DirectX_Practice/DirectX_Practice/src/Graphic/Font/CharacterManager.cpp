#include "CharacterManager.h"
#include "../SpriteManager.h"
#include "../Sprite.h"
#include "CharacterTexture.h"
#include "../../DirectX/DirectX11.h"
#include "../../util/math/MathHelper.h"
#include "../../Shader/ShaderManager.h"

#include "../../Vertex.h"
#include "../../ConstantBuffer/ConstantBuffer.h"

CharacterManager::CharacterManager() :
	m_FontSize(0),
	m_FontTexture(nullptr),
	m_RowCount(0),
	m_ColCount(0)
{
	m_FontMap.clear();
	DirectX11::GetInstance()->CreateBuffer(&m_pConstantBuffer, nullptr, sizeof(UV_CB), D3D11_BIND_CONSTANT_BUFFER);
}

CharacterManager::~CharacterManager()
{
}

void CharacterManager::Create(const std::string & fileName, int fontSize)
{
	// フォント保存用アトラステクスチャの生成
	UINT size = 256;
	m_FontTexture.Create(size, size);
	m_FontTexture.Clear();

	m_RowCount = 0;
	m_ColCount = 0;

	m_FontName = fileName;
	m_FontSize = fontSize;

	/* フォントテキストを生成 */
	// 数字データ
	const std::string number = "0123456789";
	CreateCharacter(number);
	// 記号
	const std::string mark = "+-*/%&.:!?";
	CreateCharacter(mark);
	// アルファベット大文字
	const std::string ul = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	CreateCharacter(ul);
	// アルファベット小文字
	const std::string ll = "abcdefghijklmnopqrstuvwxyz";
	CreateCharacter(ll);
}

void CharacterManager::Draw(Vector2 position, const std::string & text, const Effect& effect)
{
	float draw_x = position.x;

	m_FontTexture.Set(m_FontTexture);
	Vector2 size(1.0f / 21.0f, 1.0f / 11.2f);

	for (int i = 0; i < (int)text.length(); i++)
	{
		std::string word;
		// 半角全角判定
		if (IsDBCSLeadByte(text.at(i)) != 0)
			word = text.substr(i++, 2);	// 処理後に加算
		else
			word = text.at(i);

		// ポインタのコピー
		TCHAR* pt = (TCHAR*)word.c_str();

		// 文字データがなかったら生成
		if (m_FontMap.count(pt) == 0)
		{
			CreateCharacter(pt);
			m_FontTexture.Set(m_FontTexture);
		}

		effect.Begin();

		// シェーダにパラメータをセット
		UV_CB cb{
			m_FontMap.at(pt).uv,
			size,
		};
		DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);
		DirectX11::GetInstance()->GetContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer.p);

		SpriteManager::GetInstance()->DrawGraph(Vector2(draw_x, position.y), Vector2(m_FontSize, m_FontSize));
		
		effect.End();

		draw_x += m_FontSize;
	}
}

RenderTexture CharacterManager::GetRenderTexture() const
{
	return m_FontTexture;
}

void CharacterManager::CreateCharacter(TCHAR * c)
{
	m_FontTexture.Begin();

	// ビューポートの変更
	D3D11_VIEWPORT vp;
	vp.Width  = m_FontTexture.GetWidth();
	vp.Height = m_FontTexture.GetHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DirectX11::GetInstance()->GetContext()->RSSetViewports(1, &vp);

	Character character;
	character.pTexture = std::make_shared<Sprite>();

	// 文字データ生成
	CharacterTexture texture;
	character.pTexture->Create(texture.Create(m_FontName, c, 256));

	Effect effect(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::TEXTURE_SHADER);
	effect.Begin();

	// アトラステクスチャの描画
	character.pTexture->Begin();
	SpriteManager::GetInstance()->DrawGraph(Vector2(64 * m_RowCount, 64 * m_ColCount), Vector2(64, 64));

	character.uv.x = (float)m_RowCount / 20.0f;
	character.uv.y = (float)m_ColCount / 11.2f;
	m_FontMap[c] = character;

	m_RowCount++;
	if (m_RowCount >= 20)
	{
		m_RowCount = 0;
		m_ColCount++;
	}

	effect.End();

	// デフォルトのビューポート
	DirectX11::GetInstance()->CreateViewPort();
	m_FontTexture.End();
}

void CharacterManager::CreateCharacter(const std::string & str)
{
	for (int i = 0; i < (int)str.length(); i++)
	{
		std::string c;

		// 半角全角判定
		if (IsDBCSLeadByte(str.at(i)) != 0)
			c = str.substr(i++, 2);			// 処理後に加算
		else
			c = str.at(i);

		// 文字データがなかったら生成
		if (m_FontMap.count((TCHAR*)c.c_str()) == 0)
			CreateCharacter((TCHAR*)c.c_str());
	}
}