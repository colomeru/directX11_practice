#include "PixelShader.h"
#include "../DirectX/DirectX11.h"

PixelShader::PixelShader(const std::string& fileName, const std::string& entryPoint) :
	m_pPixelShader(nullptr)
{
	WCHAR wc[100];
	size_t ret;
	setlocale(LC_ALL, "japanese");
	mbstowcs_s(&ret, wc, 100, (PATH + fileName).c_str(), _TRUNCATE);

	LoadCompileFile(wc, entryPoint.c_str());
}

HRESULT PixelShader::LoadCompileFile(const WCHAR * fileName, const LPCSTR entryPoint)
{
	// �R���p�C���t���O
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif // defined(DEBUG) || defined(_DEBUG)

#if defined(NDEBUG) || defined(_NDEBUG)
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif // defined(NDEBUG) || defined(_NDEBUG)

	// �V�F�[�_�[�t�@�C���̓ǂݍ���
	HRESULT hr = D3DCompileFromFile(
		fileName,							// �V�F�[�_�[�t�@�C����
		nullptr,							// �g�p���Ȃ��ꍇNULL
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// �V�F�[�_�[�� #include ���g����悤�ɂ���
		entryPoint,							// �G���g���[�|�C���g�̃��\�b�h��
		"ps_5_0",							// �R���p�C���^�[�Q�b�g�i"vs_4_0"�Ȃǁj
		dwShaderFlags,						// �V�F�[�_�[�̃R���p�C���I�v�V����
		0,									// �G�t�F�N�g�t�@�C���̃R���p�C���I�v�V����
		&m_pBlob,								// �R���p�C�����ꂽ�R�[�h�փA�N�Z�X���邽�߂�ID3DBlob�C���^�[�t�F�[�X�̃|�C���^
		nullptr								// �R���p�C���G���[���b�Z�[�W�փA�N�Z�X���邽�߂�ID3DBlob�C���^�[�t�F�[�X�̃|�C���^
		);

	return hr;
}

HRESULT PixelShader::CreateShader()
{
	HRESULT hr = DirectX11::GetInstance()->GetDevice()->CreatePixelShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr,
		&m_pPixelShader
	);

	return hr;
}

void PixelShader::Begin()
{
	DirectX11::GetInstance()->GetContext()->PSSetShader(m_pPixelShader, nullptr, 0);
}

void PixelShader::End()
{
	DirectX11::GetInstance()->GetContext()->PSSetShader(nullptr, nullptr, 0);
}
