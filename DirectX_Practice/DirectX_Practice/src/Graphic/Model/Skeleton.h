#pragma once
#include <vector>
#include "../Reader/PMXFileReader.h"
#include "../Reader/PMDFileReader.h"
#include "../../util/math/Matrix.h"

class Animation;

class Skeleton
{
public:
	// �{�[���\����
	struct Bone {					// �{�[���\����
		char			name[32];	// �{�[����
		int				ID;			// �{�[��ID
		int				parentID;	// �e�{�[��ID
		Matrix			transform;	// �{�[���̕ϊ��s��
		Matrix			inverse;	// �{�[���̋t�s��
	};

public:
	// �R���X�g���N�^
	Skeleton();
	// �f�X�g���N�^
	~Skeleton();
	// PMD�t�@�C���̓ǂݍ���
	void Load(const mmd::PMD::PMDFile & pmd);
	// PMX�t�@�C���̓ǂݍ���
	void Load(const mmd::pmx::PMXFile & pmx);
	// �{�[������Ԃ�
	std::size_t Size() const;
	// �{�[���̃��[�J���ϊ��s����v�Z����
	void CalculateLocalMatrices(const Animation& animation, float frameNo, Matrix local[]) const;
	// �{�[���̃��[���h�ϊ��s����v�Z����
	void CalculateWorldMatrices(const Matrix& parent, const Matrix local[], Matrix world[]) const;
	// �X�L�j���O�p�̕ϊ��s����v�Z����
	void CalculateSkinnedMatrices(const Matrix world[], Matrix skinned[]) const;
	// ����
	void Clear();

private:
	// �{�[���z��
	std::vector<Bone>	m_Bones;
};