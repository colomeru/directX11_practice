#pragma once
#include "Effect.h"
#include "../Graphic/Model/Mesh.h"

class MeshShader : public Mesh::Shader
{
public:
	// �R���X�g���N�^
	explicit MeshShader(Effect & effect);
	// �V�F�[�_�[�J�n
	virtual void Begin() override;
	// �V�F�[�_�[�I��
	virtual void End() override;
	// �}�e���A���̐ݒ�
	virtual void Material(const Mesh::Material& material, ID3D11Buffer* constantBuffer, ID3D11SamplerState* sampler) override;

protected:
	Effect			m_Effect;						// �V�F�[�_�[
	Mesh::Material	m_Material;						// �}�e���A��
	CComPtr<ID3D11Buffer>		m_pConstantBuffer;	// �萔�o�b�t�@
	CComPtr<ID3D11SamplerState>	m_pSampler;			// �T���v���[
};