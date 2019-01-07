#pragma once
#include "Effect.h"
#include "../Graphic/Model/Mesh.h"

class ShadowMapShader : public Mesh::Shader
{
public:
	// �R���X�g���N�^
	explicit ShadowMapShader(Effect & effect);
	// �V�F�[�_�[�J�n
	virtual void Begin() override;
	// �V�F�[�_�[�I��
	virtual void End() override;
	// �}�e���A���̐ݒ�
	virtual void Material(const Mesh::Material& material, ID3D11Buffer* constantBuffer, ID3D11SamplerState* sampler) override;

protected:
	Effect			m_Effect;						// �V�F�[�_�[
};