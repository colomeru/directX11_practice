#pragma once
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <atlbase.h>
#include <string>

class PixelShader
{
public:
	// �R���X�g���N�^
	PixelShader(const std::string& fileName, const std::string& entryPoint = "main");
	// �f�X�g���N�^
	virtual ~PixelShader() {}
	// �ݒ�
	void Begin();
	// �I��
	void End();

	// �R�s�[�֎~
	PixelShader(const PixelShader&) = delete;
	PixelShader& operator = (const PixelShader&) = delete;

protected:
	// �ǂݍ���
	HRESULT LoadCompileFile(const WCHAR* fileName, const LPCSTR entryPoint);
	// �V�F�[�_�[�̐���
	HRESULT CreateShader();

protected:
	const std::string PATH = "shader/pixel/";
	CComPtr<ID3D11PixelShader>	m_pPixelShader;		// �s�N�Z���V�F�[�_
	CComPtr<ID3DBlob>			m_pBlob;
};
