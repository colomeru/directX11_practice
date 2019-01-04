#include "BoundingSphere.h"
#include "../Math/Matrix.h"

BoundingSphere::BoundingSphere() :
center(0.0f, 0.0f, 0.0f),
radius(0.0f)
{
}

BoundingSphere::BoundingSphere(const Vector3 & center, float radius) :
center(center),
radius(radius)
{
}

// ���W���܂܂�Ă��邩�ǂ����H
bool BoundingSphere::Contains(const Vector3 & target) const
{
	return Vector3::DistanceSquared(center, target) <= (radius * radius);
}

// ���E�����܂܂�Ă��邩�H
bool BoundingSphere::Contains(const BoundingSphere & other) const
{
	return Vector3::Distance(center, other.center) + radius <= radius;
}

// �������Ă��邩�H
bool BoundingSphere::Intersects(const BoundingSphere & other) const
{
	return Vector3::DistanceSquared(center, other.center) <= (radius + other.radius) * (radius + other.radius);
}

// ���W�ϊ�
BoundingSphere BoundingSphere::Transform(const Matrix & matrix) const
{
	return BoundingSphere(Vector3::Transform(center, matrix), radius * matrix.Scale().x);
}
