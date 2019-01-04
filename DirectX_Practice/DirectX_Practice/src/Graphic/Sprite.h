#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include <string>

class Texture;

class Sprite
{
public:
	// �R���X�g���N�^
	Sprite();
	// �f�X�g���N�^
	~Sprite();
	// �`��
	void Begin() const;
	// �e�N�X�`�����琶��
	bool Create(const std::string& filePath);
	// �e�N�X�`�����琶��
	bool Create(const Texture* pTexture);

	// �����擾
	unsigned int GetWidth() const;
	// �c���擾
	unsigned int GetHeight() const;

public:
	CComPtr<ID3D11ShaderResourceView>	m_pSRV;
	CComPtr<ID3D11SamplerState>			m_pSampler;
	Texture*							m_pTexture;

private:
	unsigned int						m_Width;
	unsigned int						m_Height;
};