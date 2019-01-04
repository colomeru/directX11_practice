#include "Sprite.h"
#include "../DirectX/DirectX11.h"
#include "../DirectX/Texture.h"
#include <WICTextureLoader.h>
#include <wincodec.h>

Sprite::Sprite() :
	m_pSRV(nullptr),
	m_pSampler(nullptr),
	m_pTexture(nullptr),
	m_Width(0),
	m_Height(0)
{
}

Sprite::~Sprite()
{
}

void Sprite::Begin() const
{
	DirectX11::GetInstance()->GetContext()->PSSetShaderResources(0, 1, &m_pSRV.p);
	DirectX11::GetInstance()->GetContext()->PSSetSamplers(0, 1, &m_pSampler.p);
}

bool Sprite::Create(const std::string & filePath)
{
	HRESULT hr;
	CComPtr<ID3D11Resource>	pRes;

	// キャスト
	WCHAR	wc[100];
	size_t	ret;
	mbstowcs_s(&ret, wc, 100, filePath.c_str(), _TRUNCATE);

	// テクスチャの読み込み
	hr = DirectX::CreateWICTextureFromFile(DirectX11::GetInstance()->GetDevice(), wc, &pRes, &m_pSRV.p);
	if (FAILED(hr))
	{
		MessageBox(NULL, _T("Sprite_Load_Error"), _T(filePath.c_str()), MB_OK);
		return false;
	}

	// ファイルデータ取得準備
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

	// decoder作ってファイルを渡す
	CComPtr<IWICBitmapDecoder> decoder;
	factory->CreateDecoderFromFilename(wc, 0
		, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("Decoder_Error"), _T(filePath.c_str()), MB_OK);
		return false;
	}

	// decoderからframeを取得
	CComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("Frame_Error"), _T(filePath.c_str()), MB_OK);
		return false;
	}

	// フレームからサイズとピクセルフォーマットとデータを得る
	hr = frame->GetSize(&m_Width, &m_Height);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("No_Size"), _T(filePath.c_str()), MB_OK);
		return false;
	}

	// サンプラステートの設定
	D3D11_SAMPLER_DESC	smpDesc;
	ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	// サンプラステートの生成
	DirectX11::GetInstance()->GetDevice()->CreateSamplerState(&smpDesc, &m_pSampler);

	return true;
}

bool Sprite::Create(const Texture * pTexture)
{
	HRESULT hr;

	// テクスチャ情報を取得する
	D3D11_TEXTURE2D_DESC texDesc;
	pTexture->Get()->GetDesc(&texDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format						= texDesc.Format;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip	= 0;
	srvDesc.Texture2D.MipLevels			= texDesc.MipLevels;

	// ShaderResourceViewを作成する
	hr = DirectX11::GetInstance()->GetDevice()->CreateShaderResourceView(pTexture->Get(), &srvDesc, &m_pSRV);

	if (!SUCCEEDED(hr))
		return false;

	// シェーダ用にサンプラを作成する
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy	= 1;
	samDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	hr = DirectX11::GetInstance()->GetDevice()->CreateSamplerState(&samDesc, &m_pSampler);

	return SUCCEEDED(hr);
}

unsigned int Sprite::GetWidth() const
{
	return m_Width;
}

unsigned int Sprite::GetHeight() const
{
	return m_Height;
}
