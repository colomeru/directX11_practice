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
	// DirectInputKeyborad初期化
	HRESULT hr = dInput->CreateDevice(GUID_SysKeyboard, &m_InputDevice, NULL);
	if (FAILED(hr))
		::MessageBox(NULL, "DirectInputKeyBoard初期化エラー", "DirectInputKeyBoard初期化エラー", MB_OK);

	// DirectInputSetFormat
	m_InputDevice->SetDataFormat(&c_dfDIKeyboard);

	// DirectInputSetCooperatilveLevel（第2引数は非アクティブ時に操作不能にする）
	m_InputDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// 軸モードの設定（相対値モードに設定）
	DIPROPDWORD diprop;
	diprop.diph.dwSize		 = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ※相対値の場合	DIPROPAXISMODE_ABS
	
	m_InputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	ZeroMemory(m_KeyBoardOldBuf, sizeof(m_KeyBoardOldBuf));
	ZeroMemory(m_KeyBoardNowBuf, sizeof(m_KeyBoardNowBuf));
	m_InputDevice->Acquire();
}

void Keyboard::Update()
{
	// 今のキー情報を保存
	memcpy(m_KeyBoardOldBuf, m_KeyBoardNowBuf, sizeof(m_KeyBoardNowBuf));
	// 今のキー情報をクリア
	ZeroMemory(&m_KeyBoardNowBuf, sizeof(m_KeyBoardNowBuf));
	m_OldDim = m_NowDim;
	// デバイスが非アクティブになった時対策
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