#pragma once
#include "DirectX/DepthStencil.h"
#include "DirectX/ShaderResource.h"
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
	// シェーダーリソースの取得
	ShaderResource GetShaderResource() const;

	// シャドウマップデータのセット
	void Set();
	// シャドウマップデータのクリア
	void Clear();


	void Update();
	void DebugDraw();

private:
	// シャドウマップ用シェーダーリソースセットスロット
	const int RES_SET_SLOT = 3;

	CComPtr<ID3D11RenderTargetView>	m_pPrevRTV;			// バックバッファ保存用
	DepthStencil					m_DS;				// デプスステンシルビュー
	CComPtr<ID3D11Buffer>			m_pConstantBuffer;	// 定数バッファ
	int								m_TexSize;			// テクスチャサイズ
	ShaderResource					m_Res;				// シェーダーリソース

	CComPtr<ID3D11Buffer> m_pShadowBiasCB;	// 定数バッファ
	float	m_ShadowThreshold;	// シャドウにするかどうかの閾値
	float	m_Bias;				// 固定バイアス
	float	m_SlopeScaledBias;	// 深度傾斜
	float	m_DepthBiasClamp;	// バイアスクランプ値
};