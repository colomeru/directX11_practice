#pragma once
#include <d3d11.h>
#include <atlbase.h>

struct ShaderResource
{
	CComPtr<ID3D11ShaderResourceView>	pSRV;
	CComPtr<ID3D11SamplerState>			pSampler;

	// �R���X�g���N�^
	ShaderResource();
	// �V�F�[�_�[�ɃZ�b�g
	void Set(int slot = 0) const;
};