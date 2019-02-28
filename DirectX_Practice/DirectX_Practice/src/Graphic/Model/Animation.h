#pragma once
#include <string>
#include <unordered_map>
#include <string>
#include "../../util/math/Vector3.h"
#include "../../util/math/Quaternion.h"
#include "../../util/math/Matrix.h"
#include <map>
#include "../Reader/VMDFileReader.h"

// �{�[���t���[��
using BoneFrame = std::map<std::string, mmd::vmd::Motions>;

class Animation
{
public:
	struct KeyFrame						// �L�[�t���[���\����
	{
		float			frameNo;		// �t���[���ԍ�
		Vector3			scale;			// �g��k��
		Quaternion		rotate;			// ��]
		Vector3			position;		// ���s�ړ�
		Matrix			matrix() const;	// �ϊ��s����v�Z����

		explicit KeyFrame() = default;
	};

public:
	// �R���X�g���N�^
	Animation();
	// �f�X�g���N�^
	~Animation();
	// �ǂݍ���
	void Load(const std::string& fileName);
	// �L�[�t���[���̎擾
	Animation::KeyFrame GetKeyFrame(const std::string& boneName, float frameNo) const;
	// �I���t���[������Ԃ�
	float EndFrame() const;
	// ����
	void Clear();

private:
	// �{�[���t���[���̐���
	BoneFrame CreateBoneFrame(const mmd::vmd::VMDFile& vmd);
	// �L�[�t���[���̓ǂݍ���
	void LoadBoneKeyFrame(const BoneFrame& boneFrame);

private:
	// �{�[���L�[�t���[��
	std::unordered_map<std::string, std::vector<KeyFrame>> m_BoneKeyFrame;

};
