#pragma once
#include <d3d11.h>
#include <atlbase.h>

class Texture;

class RenderTarget
{
public:
	// コンストラクタ
	RenderTarget();
	// デストラクタ
	~RenderTarget();
	// 作成
	HRESULT Create(const Texture* pTexture);
	// レンダーターゲットの取得
	ID3D11RenderTargetView* Get() const;

private:
	CComPtr<ID3D11RenderTargetView>	m_RenderTarget;	// レンダーターゲット
};