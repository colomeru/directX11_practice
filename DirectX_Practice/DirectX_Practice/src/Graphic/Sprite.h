#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include <string>

class Texture;

class Sprite
{
public:
	// コンストラクタ
	Sprite();
	// デストラクタ
	~Sprite();
	// 描画
	void Begin() const;
	// テクスチャから生成
	bool Create(const std::string& filePath);
	// テクスチャから生成
	bool Create(const Texture* pTexture);

	// 横幅取得
	unsigned int GetWidth() const;
	// 縦幅取得
	unsigned int GetHeight() const;

public:
	CComPtr<ID3D11ShaderResourceView>	m_pSRV;
	CComPtr<ID3D11SamplerState>			m_pSampler;
	Texture*							m_pTexture;

private:
	unsigned int						m_Width;
	unsigned int						m_Height;
};