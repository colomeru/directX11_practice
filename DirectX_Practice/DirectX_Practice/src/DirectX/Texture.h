#pragma once
#include <d3d11.h>
#include <atlbase.h>

class Texture
{
public:
	// �R���X�g���N�^
	Texture();
	// �f�X�g���N�^
	~Texture();
	// �쐬
	HRESULT Create();
	// �쐬
	HRESULT Create(const D3D11_TEXTURE2D_DESC texture2DDesc);
	// �����_�[�e�N�X�`���̎擾
	ID3D11Texture2D* Get() const;

private:
	CComPtr<ID3D11Texture2D>	m_pTexture;	// �e�N�X�`��2D
};