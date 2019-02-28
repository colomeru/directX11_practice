#include "ShadowMap.h"
#include "DirectX/DirectX11.h"
#include "DirectX/Texture.h"
#include "ConstantBuffer/ConstantBuffer.h"
#include "Light/Light.h"
#include <float.h>

#include "Input/Keyboard.h"
#include "util/MyUtil.h"
#include "util/math/MathHelper.h"
#include "util/math/Vector2.h"
#include "Graphic/Font/FontManager.h"

ShadowMap::ShadowMap() :
	m_pConstantBuffer(nullptr),
	m_TexSize(0),
	m_pShadowBiasCB(nullptr),
	m_ShadowThreshold(1.0f),
	m_Bias(0.000005f),
	m_SlopeScaledBias(0.01f),
	m_DepthBiasClamp(0.1f)
{
	// 定数バッファの生成
	DirectX11::GetInstance()->CreateBuffer(&m_pConstantBuffer, nullptr, sizeof(ViewProjCB), D3D11_BIND_CONSTANT_BUFFER);
	DirectX11::GetInstance()->CreateBuffer(&m_pShadowBiasCB, nullptr, sizeof(ShadowBiasCB), D3D11_BIND_CONSTANT_BUFFER);

	// シャドウマッピング用サンプラステートの設定
	D3D11_SAMPLER_DESC	smpSDesc;
	ZeroMemory(&smpSDesc, sizeof(D3D11_SAMPLER_DESC));
	smpSDesc.AddressU		= D3D11_TEXTURE_ADDRESS_BORDER;
	smpSDesc.AddressV		= D3D11_TEXTURE_ADDRESS_BORDER;
	smpSDesc.AddressW		= D3D11_TEXTURE_ADDRESS_BORDER;
	smpSDesc.BorderColor[0] = 1.0f;
	smpSDesc.BorderColor[1] = 1.0f;
	smpSDesc.BorderColor[2] = 1.0f;
	smpSDesc.BorderColor[3] = 1.0f;
	smpSDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	smpSDesc.Filter			= D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	smpSDesc.MaxAnisotropy	= 1;
	smpSDesc.MipLODBias		= 0;
	smpSDesc.MinLOD			= -FLT_MAX;
	smpSDesc.MaxLOD			= +FLT_MAX;

	// シャドウマッピング用サンプラステートの生成
	DirectX11::GetInstance()->GetDevice()->CreateSamplerState(&smpSDesc, &m_Res.pSampler.p);
}

ShadowMap::~ShadowMap()
{
}

bool ShadowMap::Create(UINT size)
{
	m_Res.pSRV.Release();

	m_TexSize = size;

	HRESULT hr;

	// テクスチャの設定
	D3D11_TEXTURE2D_DESC std;
	ZeroMemory(&std, sizeof(D3D11_TEXTURE2D_DESC));
	std.Format				= DXGI_FORMAT_R24G8_TYPELESS;
	std.Width				= m_TexSize;
	std.Height				= m_TexSize;
	std.ArraySize			= 1;
	std.MipLevels			= 1;
	std.BindFlags			= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	std.Usage				= D3D11_USAGE_DEFAULT;
	std.CPUAccessFlags		= 0;
	std.SampleDesc.Count	= 1;
	std.SampleDesc.Quality	= 0;
	std.MiscFlags			= 0;

	// シャドウマップ用のテクスチャの生成
	Texture shadowTexture;
	hr = shadowTexture.Create(std);
	if (FAILED(hr))
		return false;

	// 深度ステンシルテクスチャを生成
	hr = m_DS.Create(&shadowTexture, DXGI_FORMAT_D24_UNORM_S8_UINT);
	if (FAILED(hr))
		return false;

	// シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC		srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format				= DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// シェーダリソースビューの生成
	hr = DirectX11::GetInstance()->GetDevice()->CreateShaderResourceView(shadowTexture.Get(), &srvDesc, &m_Res.pSRV.p);
	if (FAILED(hr))
		return false;

	return true;
}

void ShadowMap::Begin(Effect& effect)
{
	DirectX11::GetInstance()->GetContext()->OMGetRenderTargets(1, &m_pPrevRTV.p, nullptr);

	// シャドウマッピングクリア
	DirectX11::GetInstance()->GetContext()->OMSetRenderTargets(0, nullptr, m_DS.Get());
	DirectX11::GetInstance()->GetContext()->ClearDepthStencilView(m_DS.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width	= (FLOAT)m_TexSize;
	vp.Height	= (FLOAT)m_TexSize;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	DirectX11::GetInstance()->GetContext()->RSSetViewports(1, &vp);

	// シェーダをシャドウマッピング用に
	effect.Begin();

	ViewProjCB cb{
		Matrix::Transpose(Light::GetInstance()->GetViewMatrix()),
		Matrix::Transpose(Matrix::CreatePerspectiveFieldOfView(60.0f, 1.0f, 0.1f, 1000.0f))
	};

	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	DirectX11::GetInstance()->GetContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer.p);
}

void ShadowMap::End(Effect& effect)
{
	effect.End();

	DirectX11::GetInstance()->GetContext()->OMSetRenderTargets(1, &m_pPrevRTV.p, nullptr);
	DirectX11::GetInstance()->CreateViewPort();

	m_pPrevRTV.Release();
}

ShaderResource ShadowMap::GetShaderResource() const
{
	return m_Res;
}

void ShadowMap::Set()
{
	static const Matrix g_ShadowBias = {
		0.5f,  0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f
	};
	
	auto viewproj = Light::GetInstance()->GetViewMatrix() * Matrix::CreatePerspectiveFieldOfView(60.0f, 1, 0.1f, 1000.0f);

	/* シャドウマップデータのセット */
	ShadowCB shadowCB{
		Matrix::Transpose(viewproj * g_ShadowBias)
	};
	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, NULL, &shadowCB, 0, 0);
	DirectX11::GetInstance()->GetContext()->VSSetConstantBuffers(2, 1, &m_pConstantBuffer.p);

	ShadowBiasCB biasCB{
		m_Bias,
		m_SlopeScaledBias,
		m_DepthBiasClamp,
	};
	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pShadowBiasCB, 0, NULL, &biasCB, 0, 0);
	DirectX11::GetInstance()->GetContext()->PSSetConstantBuffers(3, 1, &m_pShadowBiasCB.p);

	m_Res.Set(RES_SET_SLOT);
}

void ShadowMap::Clear()
{
	ShaderResource cleanRes;
	cleanRes.Set(RES_SET_SLOT);
}

void ShadowMap::Update()
{
	if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_K))
		m_Bias += 0.0000001f;
	else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_M))
		m_Bias -= 0.0000001f;
	m_Bias = MathHelper::Clamp(m_Bias, 0.0f, 1.0f);

	if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_J))
		m_SlopeScaledBias += 0.001f;
	else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_N))
		m_SlopeScaledBias -= 0.001f;
	m_SlopeScaledBias = MathHelper::Clamp(m_SlopeScaledBias, 0.0f, 1.0f);

	if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_H))
		m_DepthBiasClamp += 0.1f;
	else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_B))
		m_DepthBiasClamp -= 0.1f;
	m_DepthBiasClamp = MathHelper::Clamp(m_DepthBiasClamp, 0.0f, 1.0f);
}

void ShadowMap::DebugDraw()
{
	FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 320.0f), MyUtil::toString("m_Bias : K/M : %.7f", m_Bias));
	FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 340.0f), MyUtil::toString("m_SlopeScaledBias : J/N : %.2f", m_SlopeScaledBias));
	FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 360.0f), MyUtil::toString("m_DepthBiasClamp : H/B : %.2f", m_DepthBiasClamp));
}
