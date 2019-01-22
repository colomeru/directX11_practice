#pragma once
#include "../util/math/Matrix.h"

struct Vector3;

class FreeCamera
{
public:
	// �R���X�g���N�^
	FreeCamera();
	// �f�X�g���N�^
	~FreeCamera();
	// �X�V
	void Update();
	// �`��
	void Draw() const;

	// ���s�ړ�
	void Translation(const Vector3& v);

private:
	Matrix		m_posture;	// �p��
};
