#pragma once
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <atlbase.h>
#include <string>

class VertexShader
{
public:
	// �R���X�g���N�^
	VertexShader(const std::string& fileName, const std::string& entryPoint = "main");
	// ���z�f�X�g���N�^
	virtual ~VertexShader() {}
	// �J�n
	void Begin();
	// �I��
	void End();

	// �R�s�[�֎~
	VertexShader(const VertexShader&) = delete;
	VertexShader& operator = (const VertexShader&) = delete;

protected:
	// �ǂݍ���
	HRESULT LoadCompileFile(const WCHAR* fileName, const LPCSTR entryPoint);
	// �V�F�[�_�[�̐���
	HRESULT CreateShader();
	// �C���v�b�g���C�A�E�g�̐���
	virtual HRESULT CreateInputLayout() = 0;

protected:
	const std::string PATH = "shader/vertex/";
	CComPtr<ID3D11VertexShader>	m_pVertexShader;	// ���_�V�F�[�_
	CComPtr<ID3D11InputLayout>	m_pInputLayout;		// �C���v�b�g���C�A�E�g
	CComPtr<ID3DBlob>			m_pBlob;
};