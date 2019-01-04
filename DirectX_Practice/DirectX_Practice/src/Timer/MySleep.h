#pragma once
#pragma comment(lib, "winmm.lib")
#include <d3d11.h>

class MySleep
{
public:
	// コンストラクタ
	MySleep();
	// デストラクタ
	~MySleep();
	// 精度を取得（メインループ前に実行）
	bool QuerySet();
	// 待機
	void Wait();

	// FPS
	static float fps;

private:
	const float		MIN_FRAME_TIME = 1.0f / 60;
	float			m_FrameTime;
	LARGE_INTEGER	m_TimeStart;
	LARGE_INTEGER	m_TimeEnd;
	LARGE_INTEGER	m_TimeFreq;
};