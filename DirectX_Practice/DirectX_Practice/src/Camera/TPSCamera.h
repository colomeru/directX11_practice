#pragma once
#include "../util/math/Matrix.h"

struct Vector3;

class TPSCamera
{
public:
	// �R���X�g���N�^
	TPSCamera();
	// �f�X�g���N�^
	~TPSCamera();
	// �X�V
	void Update();
	// �`��
	void Draw() const;

	// ���s�ړ�
	void Translation(const Vector3& v);

private:
	Matrix		m_posture;	// �p��
};
