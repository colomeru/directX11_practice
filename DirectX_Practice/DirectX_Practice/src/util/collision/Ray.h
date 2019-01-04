#pragma once
#include "../Math/Vector3.h"

struct Matrix;

// ���C
struct Ray
{
	Vector3 position;		// ���W
	Vector3 direction;		// ����

	// �R���X�g���N�^
	Ray();
	// �R���X�g���N�^
	Ray(const Vector3& position, const Vector3& direction);
	// ���W�ϊ�
	Ray Transform(const Matrix& matrix) const;
};
