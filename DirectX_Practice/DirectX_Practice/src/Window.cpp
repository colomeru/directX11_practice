#include "Window.h"

Window::Window()
{
}

Window::~Window()
{
}

void Window::Create(const char * name, int width, int height)
{
	// ウィンドウクラスの作成
	WNDCLASSEX windowClass =
	{
		sizeof(WNDCLASSEX),				// 構造体のメモリサイズ
		CS_CLASSDC,						// ウィンドウクラスのスタイル
		Window::MessageProcedure,		// ウィンドウプロシージャのアドレス
		0L,								// 補助メモリ（通常は使用しないので０）
		0L,								// 補助メモリ（通常は使用しないので０）
		GetModuleHandle(nullptr),		// インスタンスハンドル
		nullptr,						// 使用するアイコンを指定（不要ならNULL）
		LoadCursor(nullptr, IDC_ARROW),	// 使用するマウスカーソルを指定（不要ならNULL）
		nullptr,						// ウィンドウのクライアント領域の背景色を設定（不要ならNULL）
		nullptr,						// ウィンドウのメニューを設定（不要ならNULL）
		"Game",							// ウィンドウクラス名
		nullptr							// 使用アイコンの小アイコン情報（不要ならNULL）
	};

	// 独自のアイコンの設定
	windowClass.hIcon = LoadIcon(GetModuleHandle(nullptr), "ICON");

	// レジスターに登録
	RegisterClassEx(&windowClass);

	// 矩形を作成
	SetRect(&m_Rect, 0, 0, width, height);

	// 矩形情報によりウィンドウ矩形を計算
	AdjustWindowRect(&m_Rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウの作成
	m_Handle = CreateWindow(
		"Game",
		name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_Rect.right - m_Rect.left,
		m_Rect.bottom - m_Rect.top,
		GetDesktopWindow(),
		nullptr,
		windowClass.hInstance,
		nullptr
	);

	// ウィンドウの矩形取得
	GetWindowRect(m_Handle, &m_Rect);

	// 表示
	Show();

	// ウィンドウの更新
	UpdateWindow(m_Handle);
}

void Window::Show() const
{
	// ウィンドウの表示
	ShowWindow(m_Handle, SW_SHOW);
}

HWND Window::GetHandle() const
{
	return m_Handle;
}

bool Window::MessageHandling()
{	
	// メッセージがあるか確認
	if (PeekMessage(&m_Message, nullptr, 0, 0, PM_NOREMOVE))
	{
		// メッセージ取得
		if (!GetMessage(&m_Message, nullptr, 0, 0))
			return false;

		// メッセージの読み取り
		TranslateMessage(&m_Message);

		// メッセージ処理
		DispatchMessage(&m_Message);
	}

	return true;
}

Window * Window::GetInstance()
{
	static Window instance;

	return &instance;
}

LRESULT Window::MessageProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		// 終了処理メッセージ送信
		PostQuitMessage(0);
		return 0;
	}

	// デフォルトウィンドウプロシージャ処理
	return DefWindowProc(hWnd, msg, wParam, lParam);
}