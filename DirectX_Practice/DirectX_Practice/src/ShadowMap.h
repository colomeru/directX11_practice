#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include "DirectX/DepthStencil.h"
#include "Shader/Effect.h"

class ShadowMap
{
public:
	// コンストラクタ
	ShadowMap();
	// デストラクタ
	~ShadowMap();
	// 生成
	bool Create(UINT size);

	// 開始
	void Begin(Effect& effect);
	// 終了
	void End(Effect& effect);

	// シャドウマップデータのセット
	void Set();
	// シャドウマップデータのクリア
	void Clear();


	void Update();
	void Draw();

private:
	CComPtr<ID3D11Buffer>				m_pConstantBuffer;	// 定数バッファ
	CComPtr<ID3D11ShaderResourceView>	m_pSRV;			// シェーダリソースビュー
	CComPtr<ID3D11SamplerState>			m_pSampler;		// サンプラーステート
	int									m_TexSize;		// テクスチャサイズ
	DepthStencil						m_DS;			// デプスステンシルビュー

	CComPtr<ID3D11Buffer> m_pShadowBiasCB;	// 定数バッファ
	float	m_ShadowThreshold;	// シャドウにするかどうかの閾値
	float	m_Bias;				// 固定バイアス
	float	m_SlopeScaledBias;	// 深度傾斜
	float	m_DepthBiasClamp;	// バイアスクランプ値

	CComPtr<ID3D11RenderTargetView>	m_pPrevRTV;		// バックバッファ保存用
};
