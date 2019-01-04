#pragma once
#include "../Math/Vector3.h"

// ���E��
struct BoundingSphere
{
	Vector3 center;		// ���S�̍��W
	float	radius;		// ���a

	// �R���X�g���N�^
	BoundingSphere();
	// �R���X�g���N�^
	BoundingSphere(const Vector3& center, float radius);
	// ���W���܂܂�Ă��邩�ǂ����H
	bool Contains(const Vector3& target) const;
	// ���E�����܂܂�Ă��邩�H
	bool Contains(const BoundingSphere& other) const;
	// �������Ă��邩�H
	bool Intersects(const BoundingSphere& other) const;
	// ���W�ϊ�
	BoundingSphere Transform(const Matrix& matrix) const;
};