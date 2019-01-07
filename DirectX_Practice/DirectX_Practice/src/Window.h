#pragma once
#include <d3d11.h>

class Window
{
public:
	// デストラクタ
	~Window();
	// 生成
	void Create(const char* name, int width, int height);
	// 表示
	void Show() const;
	// ハンドル取得
	HWND GetHandle() const;
	// メッセージ処理
	bool MessageHandling();
	// インスタンス
	static Window* GetInstance();
	// メッセージプロシージャ
	static LRESULT WINAPI MessageProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	// コンストラクタ
	Window();

private:
	HWND		m_Handle;	// ウィンドウハンドル
	MSG			m_Message;	// メッセージ
	RECT		m_Rect;		// 矩形
};