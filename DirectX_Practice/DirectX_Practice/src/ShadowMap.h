#pragma once
#include "DirectX/DepthStencil.h"
#include "DirectX/ShaderResource.h"
#include "Shader/Effect.h"

class ShadowMap
{
public:
	// �R���X�g���N�^
	ShadowMap();
	// �f�X�g���N�^
	~ShadowMap();
	// ����
	bool Create(UINT size);

	// �J�n
	void Begin(Effect& effect);
	// �I��
	void End(Effect& effect);
	// �V�F�[�_�[���\�[�X�̎擾
	ShaderResource GetShaderResource() const;

	// �V���h�E�}�b�v�f�[�^�̃Z�b�g
	void Set();
	// �V���h�E�}�b�v�f�[�^�̃N���A
	void Clear();


	void Update();
	void DebugDraw();

private:
	// �V���h�E�}�b�v�p�V�F�[�_�[���\�[�X�Z�b�g�X���b�g
	const int RES_SET_SLOT = 3;

	CComPtr<ID3D11RenderTargetView>	m_pPrevRTV;			// �o�b�N�o�b�t�@�ۑ��p
	DepthStencil					m_DS;				// �f�v�X�X�e���V���r���[
	CComPtr<ID3D11Buffer>			m_pConstantBuffer;	// �萔�o�b�t�@
	int								m_TexSize;			// �e�N�X�`���T�C�Y
	ShaderResource					m_Res;				// �V�F�[�_�[���\�[�X

	CComPtr<ID3D11Buffer> m_pShadowBiasCB;	// �萔�o�b�t�@
	float	m_ShadowThreshold;	// �V���h�E�ɂ��邩�ǂ�����臒l
	float	m_Bias;				// �Œ�o�C�A�X
	float	m_SlopeScaledBias;	// �[�x�X��
	float	m_DepthBiasClamp;	// �o�C�A�X�N�����v�l
};