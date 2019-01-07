#include "MeshVertexShader.h"
#include "../../DirectX/DirectX11.h"

MeshVertexShader::MeshVertexShader() :
	VertexShader("MeshVertexShader")
{
	// �V�F�[�_�[�̐���
	CreateShader();
	// �C���v�b�g���C�A�E�g�̐���
	CreateInputLayout();
}

HRESULT MeshVertexShader::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC ieDesc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// �C���v�b�g���C�A�E�g�̍쐬
	HRESULT hr = DirectX11::GetInstance()->GetDevice()->CreateInputLayout(
		ieDesc,
		ARRAYSIZE(ieDesc),
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		&m_pInputLayout
	);

	return hr;
}
