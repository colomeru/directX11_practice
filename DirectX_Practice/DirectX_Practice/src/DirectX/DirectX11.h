#pragma once
//#include <wrl.h>					// Microsoft::WRL::ComPtr
//using namespace Microsoft::WRL;

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#include <atlbase.h>
#include "../util/Singleton.h"

class RenderTarget;
class DepthStencil;

class DirectX11 : public Singleton<DirectX11>
{
public:
	// Singleton でのインスタンス作成は許可
	friend class Singleton<DirectX11>;

public:
	// 初期化
	bool Initialize();
	// レンダーターゲットの設定
	void SetRenderTarget(const RenderTarget* renderTarget, DepthStencil* depthStencil);
	// レンダーターゲットのクリア
	void ClearRenderTarget(const RenderTarget* renderTarget, float color[]);
	// デプスステンシルのクリア
	void ClearDepthStencil(DepthStencil* depthStencil, float depth, float stencil);
	// デバイスの取得
	ID3D11Device* GetDevice() const;
	// デバイスコンテキストの取得
	ID3D11DeviceContext* GetContext() const;
	// スワップチェインの取得
	IDXGISwapChain* GetSwapChain() const;

	// バッファ生成
	bool CreateBuffer(ID3D11Buffer** ppBuffer, void* resources, size_t size, D3D11_BIND_FLAG bindFlag);
	// バッファの生成
	//bool CreateBuffer(ID3D11Buffer** ppBuffer, void* resources, size_t size, BufferType type);
	// ビューポートの作成
	void CreateViewPort();

	// ラスタライザの設定
	bool SetRasterizer(D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID, D3D11_CULL_MODE cullMode = D3D11_CULL_NONE);

	// ブレンド ステートを無効にするための設定を取得する
	D3D11_RENDER_TARGET_BLEND_DESC GetNoBlendDesc();
	// 線形合成用ブレンド ステートのためのを設定を取得する
	D3D11_RENDER_TARGET_BLEND_DESC GetAlignmentBlendDesc();
	// 加算合成用ブレンド ステートのためのを設定を取得する
	D3D11_RENDER_TARGET_BLEND_DESC GetAddBlendDesc();
	// 減算合成用ブレンド ステートのためのを設定を取得する
	D3D11_RENDER_TARGET_BLEND_DESC GetSubtractBlendDesc();
	// 積算合成用ブレンド ステートのためのを設定を取得する
	D3D11_RENDER_TARGET_BLEND_DESC GetMultipleBlendDesc();
	// ブレンドステートを設定する
	bool SetBlendState(D3D11_RENDER_TARGET_BLEND_DESC BlendStateArray[], UINT NumBlendState, bool AlphaToCoverageEnable);
	//アルファブレンディング設定
	void SetAlphaBlend(bool alphaEnable, bool alphaToCoverageEnable);

protected:
	// コンテキスト
	DirectX11();
	// 仮想デストラクタ
	virtual ~DirectX11();

private:
	// アダプタの作成
	bool CreateAdapter();
	// デバイスとスワップチェインの作成
	bool CreateDeviceAndSwapChain();
	// ウィンドウアソシエーションの設定
	bool SetWindowAssociation();

private:
	D3D_DRIVER_TYPE			m_DriverType;		// ドライバータイプ
	D3D_FEATURE_LEVEL		m_FeatureLevel;		// 機能レベル

	CComPtr<ID3D11Device>			m_pDevice;			// デバイス
	CComPtr<ID3D11DeviceContext>	m_pDeviceContext;	// デバイスコンテキスト
	CComPtr<IDXGISwapChain>			m_pSwapChain;		// スワップチェイン
	CComPtr<IDXGIAdapter>			m_pAdapter;			// アダプタ
};


