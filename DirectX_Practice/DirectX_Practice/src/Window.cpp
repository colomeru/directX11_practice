#include "Window.h"

Window::Window()
{
}

Window::~Window()
{
}

void Window::Create(const char * name, int width, int height)
{
	// �E�B���h�E�N���X�̍쐬
	WNDCLASSEX windowClass =
	{
		sizeof(WNDCLASSEX),				// �\���̂̃������T�C�Y
		CS_CLASSDC,						// �E�B���h�E�N���X�̃X�^�C��
		Window::MessageProcedure,		// �E�B���h�E�v���V�[�W���̃A�h���X
		0L,								// �⏕�������i�ʏ�͎g�p���Ȃ��̂łO�j
		0L,								// �⏕�������i�ʏ�͎g�p���Ȃ��̂łO�j
		GetModuleHandle(nullptr),		// �C���X�^���X�n���h��
		nullptr,						// �g�p����A�C�R�����w��i�s�v�Ȃ�NULL�j
		LoadCursor(nullptr, IDC_ARROW),	// �g�p����}�E�X�J�[�\�����w��i�s�v�Ȃ�NULL�j
		nullptr,						// �E�B���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�i�s�v�Ȃ�NULL�j
		nullptr,						// �E�B���h�E�̃��j���[��ݒ�i�s�v�Ȃ�NULL�j
		"Game",							// �E�B���h�E�N���X��
		nullptr							// �g�p�A�C�R���̏��A�C�R�����i�s�v�Ȃ�NULL�j
	};

	// �Ǝ��̃A�C�R���̐ݒ�
	windowClass.hIcon = LoadIcon(GetModuleHandle(nullptr), "ICON");

	// ���W�X�^�[�ɓo�^
	RegisterClassEx(&windowClass);

	// ��`���쐬
	SetRect(&m_Rect, 0, 0, width, height);

	// ��`���ɂ��E�B���h�E��`���v�Z
	AdjustWindowRect(&m_Rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�B���h�E�̍쐬
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

	// �E�B���h�E�̋�`�擾
	GetWindowRect(m_Handle, &m_Rect);

	// �\��
	Show();

	// �E�B���h�E�̍X�V
	UpdateWindow(m_Handle);
}

void Window::Show() const
{
	// �E�B���h�E�̕\��
	ShowWindow(m_Handle, SW_SHOW);
}

HWND Window::GetHandle() const
{
	return m_Handle;
}

bool Window::MessageHandling()
{	
	// ���b�Z�[�W�����邩�m�F
	if (PeekMessage(&m_Message, nullptr, 0, 0, PM_NOREMOVE))
	{
		// ���b�Z�[�W�擾
		if (!GetMessage(&m_Message, nullptr, 0, 0))
			return false;

		// ���b�Z�[�W�̓ǂݎ��
		TranslateMessage(&m_Message);

		// ���b�Z�[�W����
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
		// �I���������b�Z�[�W���M
		PostQuitMessage(0);
		return 0;
	}

	// �f�t�H���g�E�B���h�E�v���V�[�W������
	return DefWindowProc(hWnd, msg, wParam, lParam);
}