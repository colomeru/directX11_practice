#pragma once
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <atlbase.h>
#include <string>

class PixelShader
{
public:
	// コンストラクタ
	PixelShader(const std::string& fileName, const std::string& entryPoint = "main");
	// デストラクタ
	virtual ~PixelShader() {}
	// 設定
	void Begin();
	// 終了
	void End();

	// コピー禁止
	PixelShader(const PixelShader&) = delete;
	PixelShader& operator = (const PixelShader&) = delete;

protected:
	// 読み込み
	HRESULT LoadCompileFile(const WCHAR* fileName, const LPCSTR entryPoint);
	// シェーダーの生成
	HRESULT CreateShader();

protected:
	const std::string PATH = "shader/pixel/";
	CComPtr<ID3D11PixelShader>	m_pPixelShader;		// ピクセルシェーダ
	CComPtr<ID3DBlob>			m_pBlob;
};
