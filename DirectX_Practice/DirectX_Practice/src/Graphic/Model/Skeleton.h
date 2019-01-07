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

private:
	// �{�[���z��
	std::vector<Bone>	m_Bones;
};