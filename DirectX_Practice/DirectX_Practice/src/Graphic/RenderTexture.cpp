#include "RenderTexture.h"
#include "../DirectX/DirectX11.h"
#include "../DirectX/Texture.h"
#include "SpriteManager.h"

RenderTexture::RenderTexture(DepthStencil * pDS) :
	m_pDS(pDS),
	pSRV(nullptr),
	pSampler(nullptr),
	m_Width(0),
	m_Height(0),
	m_pPrevRTV(nullptr)
{
}

RenderTexture::~RenderTexture()
{
}

HRESULT RenderTexture::Create(UINT width, UINT height)
{
	m_Width  = width;
	m_Height = height;

	HRESULT hr;

	// �Q�����e�N�X�`���̐ݒ�
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Usage			   = D3D11_USAGE_DEFAULT;
	textureDesc.Format			   = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	textureDesc.BindFlags		   = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Width			   = m_Width;
	textureDesc.Height			   = m_Height;
	textureDesc.CPUAccessFlags	   = 0;
	textureDesc.MipLevels		   = 1;
	textureDesc.ArraySize		   = 1;
	textureDesc.SampleDesc.Count   = 1;
	textureDesc.SampleDesc.Quality = 0;

	// �Q�����e�N�X�`���̐���
	Texture pTexture;
	hr = pTexture.Create(textureDesc);
	if (FAILED(hr))
		return hr;

	// �����_�[�^�[�Q�b�g�̐���
	hr = m_RT.Create(&pTexture);
	if (FAILED(hr))
		return hr;

	// �V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// �V�F�[�_���\�[�X�r���[�̐���
	hr = DirectX11::GetInstance()->GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pSRV);
	if (FAILED(hr))
		return hr;

	// �T���v���X�e�[�g�̐ݒ�
	D3D11_SAMPLER_DESC smpDesc;
	ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
	smpDesc.Filter		   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU	   = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressV	   = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressW	   = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	smpDesc.MinLOD		   = 0;
	smpDesc.MaxLOD		   = D3D11_FLOAT32_MAX;

	// �T���v���X�e�[�g�̐���
	hr = DirectX11::GetInstance()->GetDevice()->CreateSamplerState(&smpDesc, &pSampler);

	return hr;
}

void RenderTexture::Begin()
{
	DirectX11::GetInstance()->GetContext()->OMGetRenderTargets(1, &m_pPrevRTV.p, nullptr);
	DirectX11::GetInstance()->SetRenderTarget(&m_RT, m_pDS);
}

void RenderTexture::End(UINT slot)
{
	CComPtr<ID3D11ShaderResourceView> pNullSRV;
	CComPtr<ID3D11SamplerState> pNullSampler;

	DirectX11::GetInstance()->GetContext()->PSSetShaderResources(slot, 1, &pNullSRV.p);
	DirectX11::GetInstance()->GetContext()->PSSetSamplers(slot, 1, &pNullSampler.p);

	if (m_pPrevRTV)
	{
		DirectX11::GetInstance()->GetContext()->OMSetRenderTargets(1, &m_pPrevRTV.p, nullptr);
		m_pPrevRTV.Release();
	}
}

void RenderTexture::Set(const RenderTexture & pTexture, UINT slot)
{
	DirectX11::GetInstance()->GetContext()->PSSetShaderResources(slot, 1, &pTexture.pSRV.p);
	DirectX11::GetInstance()->GetContext()->PSSetSamplers(slot, 1, &pTexture.pSampler.p);
}

void RenderTexture::Clear()
{
	// �����_�[�^�[�Q�b�g�r���[�̐؂�ւ�
	if (m_pDS != nullptr)
	{
		static float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		DirectX11::GetInstance()->ClearRenderTarget(&m_RT, clearColor);
		DirectX11::GetInstance()->ClearDepthStencil(m_pDS, 1.0f, 0);
	}
	else
	{
		static float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX11::GetInstance()->ClearRenderTarget(&m_RT, clearColor);
	}
}

void RenderTexture::Draw()
{
	SpriteManager::GetInstance()->DrawGraph(0.0f, 0.0f, m_Width, m_Height);
}

int RenderTexture::GetWidth() const
{
	return m_Width;
}

int RenderTexture::GetHeight() const
{
	return m_Height;
}
