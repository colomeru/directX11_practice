#pragma once
#include <d3d11.h>
#include <atlbase.h>

class Texture;

class DepthStencil
{
public:
	// コンストラクタ
	DepthStencil();
	// デストラクタ
	~DepthStencil();
	// 作成
	HRESULT Create(const Texture* pTexture, const DXGI_FORMAT& format);
	// デプスステンシルの取得
	ID3D11DepthStencilView* Get() const;

private:
	CComPtr<ID3D11DepthStencilView>	m_DepthStencil;	// デプスステンシル
};