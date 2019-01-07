#include "DirectX11.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "../Window.h"
#include "../Utility.h"

DirectX11::DirectX11() :
	m_DriverType(D3D_DRIVER_TYPE_NULL),
	m_FeatureLevel(D3D_FEATURE_LEVEL_11_0),
	m_pDevice(nullptr),
	m_pDeviceContext(nullptr),
	m_pSwapChain(nullptr),
	m_pAdapter(nullptr)
{
}

DirectX11::~DirectX11()
{
}

bool DirectX11::Initialize()
{
	// アダプタの作成
	if (!CreateAdapter())
		return false;

	// デバイスとスワップチェインの作成
	if (!CreateDeviceAndSwapChain())
		return false;

	// ウィンドウの自動切り替えを無効化
	CreateViewPort();

	return true;
}

void DirectX11::SetRenderTarget(const RenderTarget * renderTarget, DepthStencil * depthStencil)
{
	auto target = renderTarget->Get();

	if (depthStencil)
		m_pDeviceContext->OMSetRenderTargets(1, &target, depthStencil->Get());
	else
		m_pDeviceContext->OMSetRenderTargets(1, &target, nullptr);
}

void DirectX11::ClearRenderTarget(const RenderTarget * renderTarget, float color[])
{
	m_pDeviceContext->ClearRenderTargetView(renderTarget->Get(), color);
}

void DirectX11::ClearDepthStencil(DepthStencil * depthStencil, float depth, float stencil)
{
	m_pDeviceContext->ClearDepthStencilView(depthStencil->Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

ID3D11Device * DirectX11::GetDevice() const
{
	return m_pDevice;
}

ID3D11DeviceContext * DirectX11::GetContext() const
{
	return m_pDeviceContext;
}

IDXGISwapChain * DirectX11::GetSwapChain() const
{
	return m_pSwapChain;
}

bool DirectX11::CreateBuffer(ID3D11Buffer ** ppBuffer, void * resources, size_t size, D3D11_BIND_FLAG bindFlag)
{
	HRESULT					hr;
	D3D11_BUFFER_DESC		bufferDesc;
	D3D11_SUBRESOURCE_DATA* subResoreData = nullptr;

	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = bindFlag;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// サブリソース
	if (resources)
	{
		subResoreData = new D3D11_SUBRESOURCE_DATA();
		subResoreData->pSysMem = resources;
		subResoreData->SysMemPitch = 0;
		subResoreData->SysMemSlicePitch = 0;
	}

	hr = m_pDevice->CreateBuffer(&bufferDesc, subResoreData, ppBuffer);

	delete subResoreData;

	return SUCCEEDED(hr);
}

bool DirectX11::SetRasterizer(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
{
	HRESULT hr;
	D3D11_RASTERIZER_DESC hRasterizerDesc = {};
	CComPtr<ID3D11RasterizerState>	pRasterizerState;

	hRasterizerDesc.FillMode = fillMode;
	hRasterizerDesc.CullMode = cullMode;
	hRasterizerDesc.FrontCounterClockwise = TRUE;
	hRasterizerDesc.DepthClipEnable = TRUE;
	hr = DirectX11::GetInstance()->GetDevice()->CreateRasterizerState(&hRasterizerDesc, &pRasterizerState);

	if (FAILED(hr))
		return false;;

	m_pDeviceContext->RSSetState(pRasterizerState);

	return true;
}

D3D11_RENDER_TARGET_BLEND_DESC DirectX11::GetNoBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable			= FALSE;
	RenderTarget.SrcBlend				= D3D11_BLEND_ONE;
	RenderTarget.DestBlend				= D3D11_BLEND_ZERO;
	RenderTarget.BlendOp				= D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha			= D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha			= D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

D3D11_RENDER_TARGET_BLEND_DESC DirectX11::GetAlignmentBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable			= TRUE;
	RenderTarget.SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	RenderTarget.DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	RenderTarget.BlendOp				= D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha			= D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha			= D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

D3D11_RENDER_TARGET_BLEND_DESC DirectX11::GetAddBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable			= TRUE;
	RenderTarget.SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	RenderTarget.DestBlend				= D3D11_BLEND_ONE;
	RenderTarget.BlendOp				= D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha			= D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha			= D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

D3D11_RENDER_TARGET_BLEND_DESC DirectX11::GetSubtractBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable			= TRUE;
	RenderTarget.SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	RenderTarget.DestBlend				= D3D11_BLEND_ONE;
	RenderTarget.BlendOp				= D3D11_BLEND_OP_REV_SUBTRACT;
	RenderTarget.SrcBlendAlpha			= D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha			= D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

D3D11_RENDER_TARGET_BLEND_DESC DirectX11::GetMultipleBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable			= TRUE;
	RenderTarget.SrcBlend				= D3D11_BLEND_ZERO;
	RenderTarget.DestBlend				= D3D11_BLEND_SRC_COLOR;
	RenderTarget.BlendOp				= D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha			= D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha			= D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

bool DirectX11::SetBlendState(D3D11_RENDER_TARGET_BLEND_DESC BlendStateArray[], UINT NumBlendState, bool AlphaToCoverageEnable)
{
	HRESULT hr = E_FAIL;

	CComPtr<ID3D11BlendState> pBlendState;

	if (NumBlendState > 8)
		return false;

	D3D11_BLEND_DESC BlendDesc;
	::ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;
	// TRUEの場合、マルチレンダーターゲットで各レンダーターゲットのブレンドステートの設定を個別に設定できる
	// FALSEの場合、0番目のみが使用される
	BlendDesc.IndependentBlendEnable = TRUE;

	for (UINT i = 0; i<NumBlendState; i++)
	{
		::CopyMemory(&BlendDesc.RenderTarget[i], &BlendStateArray[i], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	}

	hr = m_pDevice->CreateBlendState(&BlendDesc, &pBlendState);
	if (FAILED(hr))
		return false;

	m_pDeviceContext->OMSetBlendState(pBlendState, 0, 0xffffffff);

	return true;
}

void DirectX11::SetAlphaBlend(bool alphaEnable, bool alphaToCoverageEnable)
{
	D3D11_RENDER_TARGET_BLEND_DESC BlendDesc;
	if (alphaEnable)
		BlendDesc = GetAlignmentBlendDesc();
	else
		BlendDesc = GetNoBlendDesc();

	if (alphaToCoverageEnable)BlendDesc = GetNoBlendDesc();
	SetBlendState(&BlendDesc, 1, alphaToCoverageEnable);
}

bool DirectX11::CreateAdapter()
{
	IDXGIFactory* factory = nullptr;

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&factory))))
		return false;

	// 0番目のアダプタを取得
	if (FAILED(factory->EnumAdapters(0, &m_pAdapter)))
		return false;

	return true;
}

bool DirectX11::CreateDeviceAndSwapChain()
{
	// ドライバータイプ
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_SOFTWARE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	// ビデオカードの性能を列挙してデバイス制作時に上からの順に検索して使用可能なものが選択される
	const int FEATURE_LEVEL_NUM = 6;
	D3D_FEATURE_LEVEL featureLevels[FEATURE_LEVEL_NUM] =
	{
		D3D_FEATURE_LEVEL_11_0,  // Direct3D 11.0 SM 5
		D3D_FEATURE_LEVEL_10_1,  // Direct3D 10.1 SM 4
		D3D_FEATURE_LEVEL_10_0,  // Direct3D 10.0 SM 4
		D3D_FEATURE_LEVEL_9_3,   // Direct3D 9.3  SM 3
		D3D_FEATURE_LEVEL_9_2,   // Direct3D 9.2  SM 2
		D3D_FEATURE_LEVEL_9_1,   // Direct3D 9.1  SM 2
	};

	unsigned int createDeviceFlags = 0;

#ifndef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !_DEBUG

	UINT numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);

	// ウィンドウの画面サイズを取得
	RECT rect;
	GetClientRect(Window::GetInstance()->GetHandle(), &rect);
	UINT width  = rect.right  - rect.left;
	UINT height = rect.bottom - rect.top;

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC scDesc;
	ZeroMemory(&scDesc, sizeof(scDesc));								// 構造体を０クリア
	scDesc.BufferCount = 1;												// スワップチェーンのバッファ数
	scDesc.BufferDesc.Width		= width;								// スワップチェーンのバッファサイズ
	scDesc.BufferDesc.Height	= height;								// スワップチェーンのバッファサイズ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// スワップチェーンのバッファフォーマット
	scDesc.BufferDesc.RefreshRate.Numerator	  = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;						// 1 / 60 = 60fps
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;	// バッファをレンダーターゲットとして使用
	scDesc.OutputWindow			= Window::GetInstance()->GetHandle();	// HWNDハンドル
	scDesc.SampleDesc.Count		= 1;									// マルチサンプリングのピクセル単位
	scDesc.SampleDesc.Quality	= 0;									// マルチサンプリングの品質
	scDesc.Windowed				= TRUE;									// ウインドウモード

	HRESULT hr  = S_OK;
	bool result = false;

	for (UINT idx = 0; idx < numDriverTypes; ++idx)
	{
		// ドライバータイプ設定
		m_DriverType = driverTypes[idx];

		// デバイスとスワップチェインの生成
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,				// アダプタ－のポインタ。NULLだと規定のアダプタ
			m_DriverType,		// ハードウェアが使うD3D_DRIVER_TYPE_HARDWAREを指定
			NULL,				// DeviceTypeがハードウェアの場合NULL
			createDeviceFlags,	// 有効にするランタイムレイヤー
			featureLevels,		// 作成を試みる機能レベルの順序を指定する配列
			numFeatureLevels,	// 作成を試みる機能レベルの順序を指定する配列の数
			D3D11_SDK_VERSION,	// D3D11_SDK_VERSIONの設定
			&scDesc,			// スワップチェーンの初期化パラメーター
			&m_pSwapChain,		// 作成されるスワップチェーン
			&m_pDevice,			// 作成されるデバイス
			&m_FeatureLevel,	// 作成されるデバイスの機能レベル
			&m_pDeviceContext	// 作成されるデバイスコンテキスト
			);

		// 成功したらループ脱出
		if (SUCCEEDED(hr))
		{
			result = true;
			break;
		}
	}

	return result;
}

bool DirectX11::SetWindowAssociation()
{
	IDXGIFactory* factory = nullptr;

	if (FAILED(m_pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)(&factory))))
		return false;

	// デバイス作成のときにm_pAdapterを設定していない場合この処理を行ってもAlt + Enterでフルスクリーン切り替えが無効にならない
	if (FAILED(factory->MakeWindowAssociation(Window::GetInstance()->GetHandle(), DXGI_MWA_NO_WINDOW_CHANGES)))
		return false;

	return true;
}

void DirectX11::CreateViewPort()
{
	DXGI_SWAP_CHAIN_DESC scDesc;

	m_pSwapChain->GetDesc(&scDesc);

	D3D11_VIEWPORT vp;
	vp.Width  = (FLOAT)scDesc.BufferDesc.Width;
	vp.Height = (FLOAT)scDesc.BufferDesc.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_pDeviceContext->RSSetViewports(1, &vp);
}
