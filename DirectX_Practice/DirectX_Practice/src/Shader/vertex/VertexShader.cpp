#include "VertexShader.h"
#include "../../DirectX/DirectX11.h"

VertexShader::VertexShader(const std::string & fileName, const std::string& entryPoint)
{
	WCHAR wc[100];
	size_t ret;
	setlocale(LC_ALL, "japanese");
	mbstowcs_s(&ret, wc, 100, (PATH + fileName).c_str(), _TRUNCATE);

	LoadCompileFile(wc, entryPoint.c_str());
}

void VertexShader::Begin()
{
	DirectX11::GetInstance()->GetContext()->IASetInputLayout(m_pInputLayout);
	DirectX11::GetInstance()->GetContext()->VSSetShader(m_pVertexShader, nullptr, 0);
}

void VertexShader::End()
{
	DirectX11::GetInstance()->GetContext()->VSSetShader(nullptr, nullptr, 0);
}

HRESULT VertexShader::LoadCompileFile(const WCHAR * fileName, const LPCSTR entryPoint)
{
	// コンパイルフラグ
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif // defined(DEBUG) || defined(_DEBUG)

#if defined(NDEBUG) || defined(_NDEBUG)
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif // defined(NDEBUG) || defined(_NDEBUG)

	// シェーダーファイルの読み込み
	HRESULT hr = D3DCompileFromFile(
		fileName,							// シェーダーファイル名
		nullptr,							// 使用しない場合NULL
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// シェーダーで #include を使えるようにする
		entryPoint,							// エントリーポイントのメソッド名
		"vs_5_0",							// コンパイルターゲット（"vs_4_0"など）
		dwShaderFlags,						// シェーダーのコンパイルオプション
		0,									// エフェクトファイルのコンパイルオプション
		&m_pBlob,								// コンパイルされたコードへアクセスするためのID3DBlobインターフェースのポインタ
		nullptr								// コンパイルエラーメッセージへアクセスするためのID3DBlobインターフェースのポインタ
	);

	return hr;
}

HRESULT VertexShader::CreateShader()
{
	HRESULT hr = DirectX11::GetInstance()->GetDevice()->CreateVertexShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr,
		&m_pVertexShader
	);

	return hr;
}
