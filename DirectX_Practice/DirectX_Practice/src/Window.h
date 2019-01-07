#pragma once
#include <d3d11.h>

class Window
{
public:
	// �f�X�g���N�^
	~Window();
	// ����
	void Create(const char* name, int width, int height);
	// �\��
	void Show() const;
	// �n���h���擾
	HWND GetHandle() const;
	// ���b�Z�[�W����
	bool MessageHandling();
	// �C���X�^���X
	static Window* GetInstance();
	// ���b�Z�[�W�v���V�[�W��
	static LRESULT WINAPI MessageProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	// �R���X�g���N�^
	Window();

private:
	HWND		m_Handle;	// �E�B���h�E�n���h��
	MSG			m_Message;	// ���b�Z�[�W
	RECT		m_Rect;		// ��`
};