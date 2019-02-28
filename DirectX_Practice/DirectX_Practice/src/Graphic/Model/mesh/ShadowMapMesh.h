#pragma once
#include "../Mesh.h"

class ShadowMapMesh : public Mesh
{
	// �X�L�j���O���b�V���V�F�[�_�[
	class Shader : public Mesh::Shader {
	public:
		// ���z�f�X�g���N�^
		virtual ~Shader() {};
	};

public:
	// �R���X�g���N�^
	explicit ShadowMapMesh(Mesh& mesh);
	// �`��
	void Draw(ShadowMapMesh::Shader& shader) const;


private:
	// ���b�V��
	const Mesh*			m_Mesh;
};
