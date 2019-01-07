#pragma once
#include <d3d11.h>
#include <atlbase.h>

class Texture;

class RenderTarget
{
public:
	// �R���X�g���N�^
	RenderTarget();
	// �f�X�g���N�^
	~RenderTarget();
	// �쐬
	HRESULT Create(const Texture* pTexture);
	// �����_�[�^�[�Q�b�g�̎擾
	ID3D11RenderTargetView* Get() const;

private:
	CComPtr<ID3D11RenderTargetView>	m_RenderTarget;	// �����_�[�^�[�Q�b�g
};