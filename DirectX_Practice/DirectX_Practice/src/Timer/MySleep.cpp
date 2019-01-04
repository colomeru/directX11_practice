#include "MySleep.h"
#include <sstream>

float MySleep::fps = 0;

MySleep::MySleep() :
	m_FrameTime(0.0f),
	m_TimeStart(),
	m_TimeEnd(),
	m_TimeFreq()
{
	// 一度取得しておく
	QueryPerformanceCounter(&m_TimeStart);
}

MySleep::~MySleep()
{
}

bool MySleep::QuerySet()
{
	GetTickCount();
	auto result = QueryPerformanceFrequency(&m_TimeFreq);
	return result;
}

void MySleep::Wait()
{
	// 今の時間を取得
	QueryPerformanceCounter(&m_TimeEnd);
	// （今の時間　－　前のフレームの時間）/ 周波数 = 経過時間
	m_FrameTime = static_cast<float>(m_TimeEnd.QuadPart - m_TimeStart.QuadPart) / static_cast<float>(m_TimeFreq.QuadPart);

	if (m_FrameTime < MIN_FRAME_TIME)
	{
		// ミリ秒に変換
		DWORD sleepTime = static_cast<DWORD>((MIN_FRAME_TIME - m_FrameTime) * 1000);

		timeBeginPeriod(1);	// 分解能を上げる（Sleepの精度をあげる）
		Sleep(sleepTime);
		timeEndPeriod(1);	// 戻す

		return;
	}

	if (m_FrameTime > 0.0f)
	{
		fps = (fps * 0.99f) + (0.01f / m_FrameTime);	// 平均FPSを計算

#ifdef _DEBUG
	#ifdef UNICODE
			std::wstringstream stream;
	#else
			std::stringstream stream;
	#endif // UNICODE
			stream << fps << " FPS" << std::endl;
			OutputDebugString(stream.str().c_str());
#endif // _DEBUG
	}

	m_TimeStart = m_TimeEnd;
}
