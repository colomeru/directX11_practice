#include "DepthStencil.h"
#include "Texture.h"
#include "DirectX11.h"

DepthStencil::DepthStencil() :
	m_DepthStencil(nullptr)
{
}

DepthStencil::~DepthStencil()
{
}

HRESULT DepthStencil::Create(const Texture * pTexture, const DXGI_FORMAT& format)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	m_DepthStencil.Release();

	pTexture->Get()->GetDesc(&textureDesc);

	// デプスステンシルビューの設定
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	//depthStencilViewDesc.Format				= textureDesc.Format;
	depthStencilViewDesc.Format				= format;
	depthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	depthStencilViewDesc.Texture2DArray.ArraySize = 1;

	return DirectX11::GetInstance()->GetDevice()->CreateDepthStencilView(pTexture->Get(), &depthStencilViewDesc, &m_DepthStencil);
}

ID3D11DepthStencilView * DepthStencil::Get() const
{
	return m_DepthStencil;
}
