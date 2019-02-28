#pragma once
#include "../Graphic/Model/mesh/SkinnedMesh.h"
#include "MeshShader.h"

//
class SkinnedMeshShader : public MeshShader, public SkinnedMesh::Shader
{
public:
	// �R���X�g���N�^
	explicit SkinnedMeshShader(Effect& effect);
	// �V�F�[�_�[�J�n
	virtual void Begin() override;
	// �V�F�[�_�[�I��
	virtual void End() override;
	// �}�e���A���̐ݒ�
	virtual void Material(const Mesh::Material& material, ID3D11Buffer* constantBuffer, ID3D11SamplerState* sampler) override;
	// �{�[���̕ϊ��s���ݒ�
	virtual void BoneMatrices(int size, const Matrix matrices[]) override;

private:
	// �{�[���̍ő吔
	static const unsigned int BONE_MAX = 256;
	// �{�[����
	int			m_NumBones;
	// �{�[���̕ϊ��s��
	Matrix		m_BoneMatrices[BONE_MAX];
};
