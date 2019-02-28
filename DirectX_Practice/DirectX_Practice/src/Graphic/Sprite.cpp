#include "Sprite.h"
#include "../DirectX/DirectX11.h"
#include "../DirectX/Texture.h"
#include "../WICTextureLoader/WICTextureLoader.h"
#include <wincodec.h>

Sprite::Sprite() :
	m_Width(0),
	m_Height(0)
{
}

Sprite::~Sprite()
{
}

void Sprite::Begin() const
{
	m_Res.Set();
}

void Sprite::End()
{
	ShaderResource cleanRes;
	cleanRes.Set();
}

bool Sprite::Create(const std::string & filePath)
{
	HRESULT hr;

	// �L���X�g
	WCHAR	wc[100];
	size_t	ret;
	mbstowcs_s(&ret, wc, 100, filePath.c_str(), _TRUNCATE);

	// �e�N�X�`���̓ǂݍ���
	CComPtr<ID3D11Resource>	pRes;
	hr = DirectX::CreateWICTextureFromFile(DirectX11::GetInstance()->GetDevice(), wc, &pRes, &m_Res.pSRV.p);
	if (FAILED(hr))
	{
		MessageBox(NULL, _T("Sprite_Load_Error"), _T(filePath.c_str()), MB_OK);
		return false;
	}

	// �t�@�C���f�[�^�擾����
	CComPtr<IWICImagingFactory> factory;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&factory)
	);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("Factory_Error"), _T(filePath.c_str()), MB_OK);
		return false;
	}

	// decoder����ăt�@�C����n��
	CComPtr<IWICBitmapDecoder> decoder;
	factory->CreateDecoderFromFilename(wc, 0
		, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("Decoder_Error"), _T(filePath.c_str()), MB_OK);
		return false;
	}

	// decoder����frame���擾
	CComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("Frame_Error"), _T(filePath.c_str()), MB_OK);
		return false;
	}

	// �t���[������T�C�Y�ƃs�N�Z���t�H�[�}�b�g�ƃf�[�^�𓾂�
	hr = frame->GetSize(&m_Width, &m_Height);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("No_Size"), _T(filePath.c_str()), MB_OK);
		return false;
	}

	// �T���v���X�e�[�g�̐ݒ�
	D3D11_SAMPLER_DESC	smpDesc;
	ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	// �T���v���X�e�[�g�̐���
	hr = DirectX11::GetInstance()->GetDevice()->CreateSamplerState(&smpDesc, &m_Res.pSampler.p);
	if (FAILED(hr))
		return false;

	return true;
}

bool Sprite::Create(const Texture * pTexture)
{
	HRESULT hr;

	// �e�N�X�`�������擾����
	D3D11_TEXTURE2D_DESC texDesc;
	pTexture->Get()->GetDesc(&texDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format						= texDesc.Format;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip	= 0;
	srvDesc.Texture2D.MipLevels			= texDesc.MipLevels;

	// ShaderResourceView���쐬����
	hr = DirectX11::GetInstance()->GetDevice()->CreateShaderResourceView(pTexture->Get(), &srvDesc, &m_Res.pSRV.p);

	if (FAILED(hr))
		return false;

	// �V�F�[�_�p�ɃT���v�����쐬����
	D3D11_SAMPLER_DESC smpDesc;
	ZeroMemory(&smpDesc, sizeof(smpDesc));
	smpDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.MaxAnisotropy	= 1;
	smpDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
	smpDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	hr = DirectX11::GetInstance()->GetDevice()->CreateSamplerState(&smpDesc, &m_Res.pSampler.p);
	if (FAILED(hr))
		return false;

	return true;
}

unsigned int Sprite::GetWidth() const
{
	return m_Width;
}

unsigned int Sprite::GetHeight() const
{
	return m_Height;
}

ShaderResource Sprite::GetShaderResource() const
{
	return m_Res;
}
