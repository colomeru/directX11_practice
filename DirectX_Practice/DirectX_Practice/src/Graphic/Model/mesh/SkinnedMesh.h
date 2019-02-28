#pragma once
#include "../Mesh.h"
#include "../Animation.h"
#include "../Skeleton.h"
#include <atlbase.h>

//
class SkinnedMesh
{
public:
	//
	class Shader : public Mesh::Shader
	{
	public:
		// ���z�f�X�g���N�^
		virtual ~Shader() = default;
		// �{�[���̕ϊ��s���ݒ�
		virtual void BoneMatrices(int size, const Matrix matrices[]) = 0;
	};

public:
	// �R���X�g���N�^
	SkinnedMesh(Mesh& mesh, Skeleton& skeleton, Animation& animation);
	// �f�X�g���N�^
	~SkinnedMesh();
	//
	void Calculate(const Matrix& world, float frameNo);
	//
	void Draw(SkinnedMesh::Shader& shader) const;

private:
	// �{�[���̍ő吔
	static const unsigned int BONE_MAX = 256;
	// ���b�V��
	const Mesh*				m_pMesh;
	// �X�P���g��
	const Skeleton*			m_pSkeleton;
	// �A�j���[�V����
	const Animation*		m_pAnimation;
	// ���[�J���ϊ��s��
	Matrix					m_LocalMatrices[BONE_MAX];
	// ���[���h�ϊ��s��
	Matrix					m_WorldMatrices[BONE_MAX];
	// �X�L�j���O�ϊ��s��
	Matrix					m_SkinnedMatrices[BONE_MAX];
	// �{�[����񑗐M�p�R���X�^���g�o�b�t�@
	CComPtr<ID3D11Buffer>	m_pConstantBuffer;
};
