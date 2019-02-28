#pragma once
#include "../DirectX/RenderTarget.h"
#include "../DirectX/DepthStencil.h"
#include "../DirectX/ShaderResource.h"
#include <string>

class RenderTexture
{
public:
	// コンストラクタ
	explicit RenderTexture(DepthStencil* pDS);
	// デストラクタ
	~RenderTexture();
	// 生成
	HRESULT Create(UINT width, UINT height);
	// ポストエフェクト開始
	void Begin();
	// ポストエフェクト終了
	void End(UINT slot = 0);
	// テクスチャのセット
	void Set(const RenderTexture& effect, UINT slot = 0);
	// クリア
	void Clear();
	// 描画
	void Draw();
	// 横幅取得
	int GetWidth() const;
	// 縦幅取得
	int GetHeight() const;
	// シェーダーリソース取得
	ShaderResource GetShaderResource() const;

private:
	RenderTarget				m_RT;				// 現在のレンダーターゲット
	DepthStencil*				m_pDS;				// 現在のデプスステンシル
	ShaderResource				m_Res;				// シェーダーリソース
	unsigned int				m_Width;			// 画面の横幅
	unsigned int				m_Height;			// 画面の縦幅

	CComPtr<ID3D11RenderTargetView>	m_pPrevRTV;		// バックバッファ保存用
};
