#pragma once
#include <d3d11.h>

class GameBase
{
public:
	// コンストラクタ
	GameBase();
	// デストラクタ
	~GameBase();

	// 実行
	bool Run(HINSTANCE hIns);

private:
	// リソース読み込み
	void LoadResources();

private:
	UINT						m_MultiSampleCount;			// マルチサンプルのカウント
	UINT						m_MultiSampleQuality;		// マルチサンプルの品質
	UINT						m_MultiSampleMaxQuality;	// マルチサンプルの最大品質
	UINT						m_SwapChainCount;			// スワップチェインのカウント
	DXGI_FORMAT					m_SwapChainFormat;			// スワップチェインのフォーマット
	DXGI_FORMAT					m_DepthStencilFormat;		// 深度ステンシルのフォーマット
	UINT						m_Width;					// ウィンドウ横幅
	UINT						m_Height;					// ウィンドウ縦幅
	FLOAT						m_AspectRatio;				// アスペクト比
};
