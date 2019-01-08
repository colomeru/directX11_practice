#pragma once
#include "../util/Singleton.h"
#include <d3d11.h>
#include <atlbase.h>
#include <string>
#include <unordered_map>
#include "../ID.h"
#include "SpritePtr.h"
#include "../Shader/Effect.h"

struct Vector2;
struct ShaderResource;
class RenderTexture;

class SpriteManager : public Singleton<SpriteManager>
{
public:
	//
	friend class Singleton<SpriteManager>;

public:
	// 初期化
	void Initialize(int width, int height);
	// 読み込み
	bool Load(const std::string& fileName, const SPRITE_ID& id);
	// クリア
	void Clear();

	// 描画（あらかじめリソースデータは送信しておく）
	void DrawGraph(Vector2 position, Vector2 size);
	// 描画（あらかじめリソースデータは送信しておく）
	void DrawGraph(float x, float y, unsigned int width, unsigned int height);
	// 描画
	void DrawGraph(Vector2 position, ID3D11ShaderResourceView* srv, ID3D11SamplerState* sampler, unsigned int width, unsigned int height);
	// 描画
	void DrawGraph(Vector2 position, const ShaderResource& res, unsigned int width, unsigned int height);
	// 描画
	void DrawGraph(Vector2 position, const RenderTexture& renderTexture);

	// 描画
	void Draw(const SpritePtr& sprite, Vector2 position, Vector2 size);
	// 描画
	void Draw(SPRITE_ID id, Vector2 position);
	// 描画
	void Draw(SPRITE_ID id, Vector2 position, Vector2 size);

protected:
	// コンストラクタ
	SpriteManager();
	// デストラクタ
 	virtual ~SpriteManager();

private:
	// 頂点データを生成しシェーダに送る
	void SetVertexBuffer();
	// 定数バッファを生成しシェーダに送る
	void SetConstantBuffer(unsigned int width, unsigned int height, Vector2 position);

private:
	const std::string PATH = "res/2D/";
	
	std::unordered_map<SPRITE_ID, SpritePtr>	m_SpriteMap;

	CComPtr<ID3D11Buffer>		m_pVertexBuffer;	// 頂点バッファ
	CComPtr<ID3D11Buffer>		m_pConstantBuffer;	// 定数バッファ

	unsigned int				m_Width;			// ウィンドウの縦幅
	unsigned int				m_Height;			// ウィンドウの横幅

	Effect						m_Effect;
};
