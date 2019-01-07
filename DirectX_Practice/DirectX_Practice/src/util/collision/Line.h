#pragma once
#include "../Math/Vector3.h"

// ����
struct Line
{
	Vector3 start;		// �n�_
	Vector3 end;		// �I�_

	// �R���X�g���N�^
	Line();
	// �R���X�g���N�^
	Line(const Vector3& start, const Vector3& end);
	// ���������߂�
	float Length() const;
	// �n�_����I�_�ւ̕�����Ԃ�
	Vector3 Direction() const;
	// ���W�ϊ�
	Line Transform(const Matrix& matrix) const;
};
