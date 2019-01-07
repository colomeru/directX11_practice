#pragma once
#pragma comment(lib, "winmm.lib")
#include <d3d11.h>

class MySleep
{
public:
	// �R���X�g���N�^
	MySleep();
	// �f�X�g���N�^
	~MySleep();
	// ���x���擾�i���C�����[�v�O�Ɏ��s�j
	bool QuerySet();
	// �ҋ@
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