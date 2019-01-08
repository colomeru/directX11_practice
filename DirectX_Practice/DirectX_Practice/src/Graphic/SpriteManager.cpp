#include "SpriteManager.h"
#include "Sprite.h"
#include "../DirectX/DirectX11.h"
#include "../DirectX/Texture.h"
#include "../DirectX/ShaderResource.h"
#include "../ConstantBuffer/ConstantBuffer.h"
#include "../Vertex.h"
#include "RenderTexture.h"
#include "../util/math/Vector2.h"
#include "../util/math/Vector3.h"

SpriteManager::SpriteManager() :
	m_pVertexBuffer(nullptr),
	m_pConstantBuffer(nullptr),
	m_Effect()
{
	// 頂点データ生成
	TextureVertex vertices[4] =
	{
		{ Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(0.0f,	1.0f, 0.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f) },
	};

	// 頂点バッファの生成
	DirectX11::GetInstance()->CreateBuffer(&m_pVertexBuffer, vertices, sizeof(TextureVertex) * 4, D3D11_BIND_VERTEX_BUFFER);
	// 定数バッファの生成
	DirectX11::GetInstance()->CreateBuffer(&m_pConstantBuffer, nullptr, sizeof(WorldProjCB), D3D11_BIND_CONSTANT_BUFFER);
}

SpriteManager::~SpriteManager()
{
}

void SpriteManager::Initialize(int width, int height)
{
	m_SpriteMap.clear();
	m_Effect.Load(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::TEXTURE_SHADER);

	m_Width  = width;
	m_Height = height;
}

bool SpriteManager::Load(const std::string & fileName, const SPRITE_ID & id)
{
	// Spriteの生成
	auto sprite = std::make_shared<Sprite>();
	sprite->Create(PATH + fileName);

	// マップにスプライトデータを登録
	m_SpriteMap[id] = sprite;

	return true;
}

SpritePtr SpriteManager::Get(SPRITE_ID id) const
{
	return m_SpriteMap.at(id);
}

void SpriteManager::Clear()
{
	m_SpriteMap.clear();
}

void SpriteManager::DrawGraph(Vector2 position, Vector2 size)
{
	// シェーダに送るパラメータの生成＆セット
	SetVertexBuffer();
	SetConstantBuffer(size.x, size.y, position);

	// 描画
	DirectX11::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DirectX11::GetInstance()->GetContext()->Draw(4, 0);
}

void SpriteManager::DrawGraph(float x, float y, unsigned int width, unsigned int height)
{
	DrawGraph(Vector2(x, y), Vector2(width, height));
}

void SpriteManager::DrawGraph(Vector2 position, ID3D11ShaderResourceView* srv, ID3D11SamplerState* sampler, unsigned int width, unsigned int height)
{
	// シェーダのセット
	m_Effect.Begin();

	// リソースデータのセット
	DirectX11::GetInstance()->GetContext()->PSSetShaderResources(0, 1, &srv);
	DirectX11::GetInstance()->GetContext()->PSSetSamplers(0, 1, &sampler);

	// シェーダに送るパラメータのセット
	SetVertexBuffer();
	SetConstantBuffer(width, height, position);

	DirectX11::GetInstance()->SetAlphaBlend(true, false);

	// 描画
	DirectX11::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DirectX11::GetInstance()->GetContext()->Draw(4, 0);

	DirectX11::GetInstance()->SetAlphaBlend(false, false);

	// 後処理
	ShaderResource cleanRes;
	cleanRes.Set();

	m_Effect.End();
}

void SpriteManager::DrawGraph(Vector2 position, const ShaderResource & res, unsigned int width, unsigned int height)
{
	DrawGraph(position, res.pSRV.p, res.pSampler.p, width, height);
}

void SpriteManager::DrawGraph(Vector2 position, const RenderTexture & renderTexture)
{
	DrawGraph(position, renderTexture.GetShaderResource(), renderTexture.GetWidth(), renderTexture.GetHeight());
}

void SpriteManager::Draw(const SpritePtr & sprite, Vector2 position, Vector2 size)
{
	m_Effect.Begin();
	DirectX11::GetInstance()->SetAlphaBlend(true, false);

	sprite->Begin();
	DrawGraph(position, size);

	DirectX11::GetInstance()->SetAlphaBlend(false, false);
	m_Effect.End();
}

void SpriteManager::Draw(SPRITE_ID id, Vector2 position)
{
	auto sprite = m_SpriteMap[id];
	Draw(sprite, position, Vector2(sprite->GetWidth(), sprite->GetHeight()));
}

void SpriteManager::Draw(SPRITE_ID id, Vector2 position, Vector2 size)
{
	Draw(m_SpriteMap[id], position, size);
}

void SpriteManager::SetVertexBuffer()
{
	unsigned int stride = sizeof(TextureVertex);;
	unsigned int offset = 0;
	// シェーダにパラメータをセット
	DirectX11::GetInstance()->GetContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer.p, &stride, &offset);
}

void SpriteManager::SetConstantBuffer(unsigned int width, unsigned int height, Vector2 position)
{
	static const Matrix proj(
		2.0f / m_Width, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / m_Height, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
		);

	Matrix world = Matrix::CreateScale(Vector3(width, height, 1.0f)) 
				 * Matrix::CreateTranslation(Vector3(position));

	WorldProjCB cb{
		Matrix::Transpose(world),
		Matrix::Transpose(proj)
	};

	// シェーダにパラメータをセット
	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);
	DirectX11::GetInstance()->GetContext()->VSSetConstantBuffers(1, 1, &m_pConstantBuffer.p);
}
