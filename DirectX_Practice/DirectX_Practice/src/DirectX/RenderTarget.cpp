#include "RenderTarget.h"
#include "Texture.h"
#include "DirectX11.h"

RenderTarget::RenderTarget() :
	m_RenderTarget(nullptr)
{
}

RenderTarget::~RenderTarget()
{
}

HRESULT RenderTarget::Create(const Texture * pTexture)
{
	D3D11_TEXTURE2D_DESC textureDesc;

	pTexture->Get()->GetDesc(&textureDesc);

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

	// �V�F�[�_�[���\�[�X�̐ݒ肪����Ă�����e�N�X�`���Ƃ��Ďg���Ă���\��������
	if (textureDesc.BindFlags != D3D11_BIND_SHADER_RESOURCE)
	{
		renderTargetViewDesc.Format		   = DXGI_FORMAT_R8G8B8A8_UNORM;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	}

	// �����_�[�^�[�Q�b�g�r���[�̐���
	return DirectX11::GetInstance()->GetDevice()->CreateRenderTargetView(pTexture->Get(), &renderTargetViewDesc, &m_RenderTarget);
}

ID3D11RenderTargetView * RenderTarget::Get() const
{
	return m_RenderTarget;
}
