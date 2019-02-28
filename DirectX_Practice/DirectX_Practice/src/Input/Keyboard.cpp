#include "Keyboard.h"
#include "../DirectX/DirectX11.h"

Keyboard::Keyboard() :
	m_KeyBoardNowBuf(),
	m_KeyBoardOldBuf()
{

}

Keyboard::~Keyboard()
{
}

void Keyboard::Initialize(HWND hWnd, LPDIRECTINPUT8 dInput)
{
	// DirectInputKeyborad������
	HRESULT hr = dInput->CreateDevice(GUID_SysKeyboard, &m_InputDevice, NULL);
	if (FAILED(hr))
		::MessageBox(NULL, "DirectInputKeyBoard�������G���[", "DirectInputKeyBoard�������G���[", MB_OK);

	// DirectInputSetFormat
	m_InputDevice->SetDataFormat(&c_dfDIKeyboard);

	// DirectInputSetCooperatilveLevel�i��2�����͔�A�N�e�B�u���ɑ���s�\�ɂ���j
	m_InputDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// �����[�h�̐ݒ�i���Βl���[�h�ɐݒ�j
	DIPROPDWORD diprop;
	diprop.diph.dwSize		 = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// �����Βl�̏ꍇ	DIPROPAXISMODE_ABS
	
	m_InputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	ZeroMemory(m_KeyBoardOldBuf, sizeof(m_KeyBoardOldBuf));
	ZeroMemory(m_KeyBoardNowBuf, sizeof(m_KeyBoardNowBuf));
	m_InputDevice->Acquire();
}

void Keyboard::Update()
{
	// ���̃L�[����ۑ�
	memcpy(m_KeyBoardOldBuf, m_KeyBoardNowBuf, sizeof(m_KeyBoardNowBuf));
	// ���̃L�[�����N���A
	ZeroMemory(&m_KeyBoardNowBuf, sizeof(m_KeyBoardNowBuf));
	m_OldDim = m_NowDim;
	// �f�o�C�X����A�N�e�B�u�ɂȂ������΍�
	if (FAILED(m_InputDevice->GetDeviceState(sizeof(m_KeyBoardNowBuf), m_KeyBoardNowBuf)))
		while (m_InputDevice->Acquire() == DIERR_INPUTLOST);
}

bool Keyboard::IsKey(const UINT keyCode) const
{
	return m_KeyBoardNowBuf[keyCode];
}

bool Keyboard::IsKeyUp(const UINT keyCode) const
{
	return !m_KeyBoardNowBuf[keyCode] && m_KeyBoardOldBuf[keyCode];
}

bool Keyboard::IsKeyDown(const UINT keyCode) const
{
	return m_KeyBoardNowBuf[keyCode] && !m_KeyBoardOldBuf[keyCode];
}