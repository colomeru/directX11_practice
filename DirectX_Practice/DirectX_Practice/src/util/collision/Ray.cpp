#include "Ray.h"
#include "../Math/Matrix.h"

// コンストラクタ
Ray::Ray() :
position(0.0f, 0.0f, 0.0f),
direction(0.0f, 0.0f, 0.0f)
{
}

// コンストラクタ
Ray::Ray(const Vector3 & position, const Vector3 & direction) :
position(position),
direction(direction)
{
}

// 座標変換
Ray Ray::Transform(const Matrix & matrix) const
{
	return Ray(Vector3::Transform(position, matrix), Vector3::TransformNormal(direction, matrix));
}
