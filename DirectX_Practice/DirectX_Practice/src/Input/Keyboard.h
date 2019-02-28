#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif
#include <dinput.h>
#include "../util/Singleton.h"

namespace KEY_CODE
{
	const UINT KEY_RIGHT = DIK_RIGHTARROW;
	const UINT KEY_LEFT  = DIK_LEFTARROW;
	const UINT KEY_UP	 = DIK_UPARROW;
	const UINT KEY_DOWN  = DIK_DOWNARROW;
	const UINT KEY_A = DIK_A;
	const UINT KEY_B = DIK_B;
	const UINT KEY_C = DIK_C;
	const UINT KEY_D = DIK_D;
	const UINT KEY_E = DIK_E;
	const UINT KEY_F = DIK_F;
	const UINT KEY_G = DIK_G;
	const UINT KEY_H = DIK_H;
	const UINT KEY_I = DIK_I;
	const UINT KEY_J = DIK_J;
	const UINT KEY_K = DIK_K;
	const UINT KEY_L = DIK_L;
	const UINT KEY_M = DIK_M;
	const UINT KEY_N = DIK_N;
	const UINT KEY_O = DIK_O;
	const UINT KEY_P = DIK_P;
	const UINT KEY_Q = DIK_Q;
	const UINT KEY_R = DIK_R;
	const UINT KEY_S = DIK_S;
	const UINT KEY_T = DIK_T;
	const UINT KEY_U = DIK_U;
	const UINT KEY_V = DIK_V;
	const UINT KEY_W = DIK_W;
	const UINT KEY_X = DIK_X;
	const UINT KEY_Y = DIK_Y;
	const UINT KEY_Z = DIK_Z;
	const UINT KEY_LSHIFT = DIK_LSHIFT;
	const UINT KEY_LCTRL  = DIK_LCONTROL;
	const UINT KEY_SPACE  = DIK_SPACE;
	const UINT KEY_1 = DIK_1;
	const UINT KEY_2 = DIK_2;
	const UINT KEY_3 = DIK_3;
	const UINT KEY_4 = DIK_4;
	const UINT KEY_5 = DIK_5;
	const UINT KEY_6 = DIK_6;
	const UINT KEY_7 = DIK_7;
	const UINT KEY_8 = DIK_8;
	const UINT KEY_9 = DIK_9;
	const UINT KEY_0 = DIK_0;
	const UINT KEY_TAB = DIK_TAB;
	const UINT KEY_ESC = DIK_ESCAPE;
	const UINT KEY_PAGEUP	= DIK_PRIOR;
	const UINT KEY_PAGEDOWN = DIK_NEXT;
}

class Keyboard : public Singleton<Keyboard>
{
public:
	//
	friend class Singleton<Keyboard>;

public:
	// 初期化
	void Initialize(HWND hWnd, LPDIRECTINPUT8 dInput);
	// 更新
	void Update();
	// キーが押されているか？
	bool IsKey(const UINT keyCode) const;
	// キーが離されたか？
	bool IsKeyUp(const UINT keyCode) const;
	// キーが押されたか？
	bool IsKeyDown(const UINT keyCode) const;

protected:
	// コンストラクタ
	Keyboard();
	// デストラクタ
	virtual ~Keyboard();

private:
	LPDIRECTINPUTDEVICE8	m_InputDevice;
	BYTE					m_KeyBoardNowBuf[256];
	BYTE					m_KeyBoardOldBuf[256];
	DIMOUSESTATE2			m_NowDim;
	DIMOUSESTATE2			m_OldDim;
};