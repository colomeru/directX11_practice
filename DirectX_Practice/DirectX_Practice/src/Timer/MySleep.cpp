#include "MySleep.h"
#include <sstream>

float MySleep::fps = 0;

MySleep::MySleep() :
	m_FrameTime(0.0f),
	m_TimeStart(),
	m_TimeEnd(),
	m_TimeFreq()
{
	// ��x�擾���Ă���
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
	// ���̎��Ԃ��擾
	QueryPerformanceCounter(&m_TimeEnd);
	// �i���̎��ԁ@�|�@�O�̃t���[���̎��ԁj/ ���g�� = �o�ߎ���
	m_FrameTime = static_cast<float>(m_TimeEnd.QuadPart - m_TimeStart.QuadPart) / static_cast<float>(m_TimeFreq.QuadPart);

	if (m_FrameTime < MIN_FRAME_TIME)
	{
		// �~���b�ɕϊ�
		DWORD sleepTime = static_cast<DWORD>((MIN_FRAME_TIME - m_FrameTime) * 1000);

		timeBeginPeriod(1);	// ����\���グ��iSleep�̐��x��������j
		Sleep(sleepTime);
		timeEndPeriod(1);	// �߂�

		return;
	}

	if (m_FrameTime > 0.0f)
	{
		fps = (fps * 0.99f) + (0.01f / m_FrameTime);	// ����FPS���v�Z

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
