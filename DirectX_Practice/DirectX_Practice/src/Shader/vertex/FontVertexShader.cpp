#include "FontVertexShader.h"
#include "../../DirectX/DirectX11.h"

FontVertexShader::FontVertexShader() :
	VertexShader("FontVertexShader.hlsl")
{
	// シェーダーの生成
	CreateShader();
	// インプットレイアウトの生成
	CreateInputLayout();
}

HRESULT FontVertexShader::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC ieDesc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// インプットレイアウトの作成
	HRESULT hr = DirectX11::GetInstance()->GetDevice()->CreateInputLayout(
		ieDesc,
		ARRAYSIZE(ieDesc),
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		&m_pInputLayout
	);

	return hr;
}
