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
		"ps_5_0",							// コンパイルターゲット（"vs_4_0"など）
		dwShaderFlags,						// シェーダーのコンパイルオプション
		0,									// エフェクトファイルのコンパイルオプション
		&m_pBlob,								// コンパイルされたコードへアクセスするためのID3DBlobインターフェースのポインタ
		nullptr								// コンパイルエラーメッセージへアクセスするためのID3DBlobインターフェースのポインタ
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
