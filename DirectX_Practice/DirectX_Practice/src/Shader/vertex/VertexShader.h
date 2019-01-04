#pragma once
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <atlbase.h>
#include <string>

class VertexShader
{
public:
	// コンストラクタ
	VertexShader(const std::string& fileName, const std::string& entryPoint = "main");
	// 仮想デストラクタ
	virtual ~VertexShader() {}
	// 開始
	void Begin();
	// 終了
	void End();

	// コピー禁止
	VertexShader(const VertexShader&) = delete;
	VertexShader& operator = (const VertexShader&) = delete;

protected:
	// 読み込み
	HRESULT LoadCompileFile(const WCHAR* fileName, const LPCSTR entryPoint);
	// シェーダーの生成
	HRESULT CreateShader();
	// インプットレイアウトの生成
	virtual HRESULT CreateInputLayout() = 0;

protected:
	const std::string PATH = "shader/vertex/";
	CComPtr<ID3D11VertexShader>	m_pVertexShader;	// 頂点シェーダ
	CComPtr<ID3D11InputLayout>	m_pInputLayout;		// インプットレイアウト
	CComPtr<ID3DBlob>			m_pBlob;
};