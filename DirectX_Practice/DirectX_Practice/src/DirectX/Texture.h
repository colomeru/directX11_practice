#pragma once
#include <d3d11.h>
#include <atlbase.h>

class Texture
{
public:
	// コンストラクタ
	Texture();
	// デストラクタ
	~Texture();
	// 作成
	HRESULT Create();
	// 作成
	HRESULT Create(const D3D11_TEXTURE2D_DESC texture2DDesc);
	// レンダーテクスチャの取得
	ID3D11Texture2D* Get() const;

private:
	CComPtr<ID3D11Texture2D>	m_pTexture;	// テクスチャ2D
};