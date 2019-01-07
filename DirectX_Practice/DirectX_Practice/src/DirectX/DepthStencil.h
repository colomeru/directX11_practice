#pragma once
#include <d3d11.h>
#include <atlbase.h>

class Texture;

class DepthStencil
{
public:
	// �R���X�g���N�^
	DepthStencil();
	// �f�X�g���N�^
	~DepthStencil();
	// �쐬
	HRESULT Create(const Texture* pTexture, const DXGI_FORMAT& format);
	// �f�v�X�X�e���V���̎擾
	ID3D11DepthStencilView* Get() const;

private:
	CComPtr<ID3D11DepthStencilView>	m_DepthStencil;	// �f�v�X�X�e���V��
};