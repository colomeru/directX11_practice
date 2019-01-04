#include "Ray.h"
#include "../Math/Matrix.h"

// �R���X�g���N�^
Ray::Ray() :
position(0.0f, 0.0f, 0.0f),
direction(0.0f, 0.0f, 0.0f)
{
}

// �R���X�g���N�^
Ray::Ray(const Vector3 & position, const Vector3 & direction) :
position(position),
direction(direction)
{
}

// ���W�ϊ�
Ray Ray::Transform(const Matrix & matrix) const
{
	return Ray(Vector3::Transform(position, matrix), Vector3::TransformNormal(direction, matrix));
}
