#include "Texture.h"
#include "DirectX11.h"
#include <tchar.h>

Texture::Texture() :
	m_pTexture(nullptr)
{
}

Texture::~Texture()
{
}

HRESULT Texture::Create()
{
	return DirectX11::GetInstance()->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pTexture);
}

HRESULT Texture::Create(const D3D11_TEXTURE2D_DESC texture2DDesc)
{
	return DirectX11::GetInstance()->GetDevice()->CreateTexture2D(&texture2DDesc, nullptr, &m_pTexture);
}

ID3D11Texture2D * Texture::Get() const
{
	return m_pTexture;
}
