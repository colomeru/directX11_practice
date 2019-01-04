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

// 座標が含まれているかどうか？
bool BoundingSphere::Contains(const Vector3 & target) const
{
	return Vector3::DistanceSquared(center, target) <= (radius * radius);
}

// 境界球が含まれているか？
bool BoundingSphere::Contains(const BoundingSphere & other) const
{
	return Vector3::Distance(center, other.center) + radius <= radius;
}

// 交差しているか？
bool BoundingSphere::Intersects(const BoundingSphere & other) const
{
	return Vector3::DistanceSquared(center, other.center) <= (radius + other.radius) * (radius + other.radius);
}

// 座標変換
BoundingSphere BoundingSphere::Transform(const Matrix & matrix) const
{
	return BoundingSphere(Vector3::Transform(center, matrix), radius * matrix.Scale().x);
}
