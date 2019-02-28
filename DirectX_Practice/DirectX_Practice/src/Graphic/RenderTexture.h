#pragma once
#include "../DirectX/RenderTarget.h"
#include "../DirectX/DepthStencil.h"
#include "../DirectX/ShaderResource.h"
#include <string>

class RenderTexture
{
public:
	// �R���X�g���N�^
	explicit RenderTexture(DepthStencil* pDS);
	// �f�X�g���N�^
	~RenderTexture();
	// ����
	HRESULT Create(UINT width, UINT height);
	// �|�X�g�G�t�F�N�g�J�n
	void Begin();
	// �|�X�g�G�t�F�N�g�I��
	void End(UINT slot = 0);
	// �e�N�X�`���̃Z�b�g
	void Set(const RenderTexture& effect, UINT slot = 0);
	// �N���A
	void Clear();
	// �`��
	void Draw();
	// �����擾
	int GetWidth() const;
	// �c���擾
	int GetHeight() const;
	// �V�F�[�_�[���\�[�X�擾
	ShaderResource GetShaderResource() const;

private:
	RenderTarget				m_RT;				// ���݂̃����_�[�^�[�Q�b�g
	DepthStencil*				m_pDS;				// ���݂̃f�v�X�X�e���V��
	ShaderResource				m_Res;				// �V�F�[�_�[���\�[�X
	unsigned int				m_Width;			// ��ʂ̉���
	unsigned int				m_Height;			// ��ʂ̏c��

	CComPtr<ID3D11RenderTargetView>	m_pPrevRTV;		// �o�b�N�o�b�t�@�ۑ��p
};
