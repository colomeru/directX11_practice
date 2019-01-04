#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"

#include "Line.h"
#include "Ray.h"
#include "../Math/MathHelper.h"
#include <cmath>
#include <algorithm>
#include <limits>

// コンストラクタ
BoundingBox::BoundingBox() :
min(Vector3::Zero),
max(Vector3::Zero)
{
}

// コンストラクタ
BoundingBox::BoundingBox(const Vector3 & min, const Vector3 & max) :
min(min),
max(max)
{
}

// 点が内包されているか？
bool BoundingBox::Contains(const Vector3 & point) const
{
	return	(min.x <= point.x && point.x <= max.x) &&
			(min.y <= point.y && point.y <= max.y) &&
			(min.z <= point.z && point.z <= max.z);
}

// 境界ボックスが内包されているか？
bool BoundingBox::Contains(const BoundingBox & other) const
{
	return  (min.x <= other.min.x && other.max.x <= max.x) &&
			(min.y <= other.min.y && other.max.y <= max.y) &&
			(min.z <= other.min.z && other.max.z <= max.z);
}

// 境界球が内包されているか？
bool BoundingBox::Contains(const BoundingSphere & sphere) const
{
	const Vector3 offset(sphere.radius, sphere.radius, sphere.radius);
	return Contains(BoundingBox(sphere.center - offset, sphere.center + offset));
}

// 線分が内包されているか？
bool BoundingBox::Contains(const Line & line) const
{
	return Contains(line.start) && Contains(line.end);
}

// OBBが内包されているか？
bool BoundingBox::Contains(const OrientedBoundingBox & obb) const
{
	return obb.Contains(*this);
}

// 境界ボックスとの交差判定
bool BoundingBox::Intersects(const BoundingBox & other) const
{
	return  min.x < other.max.x &&
			min.y < other.max.y &&
			min.z < other.max.z &&
			other.min.x < max.x &&
			other.min.y < max.y &&
			other.min.z < max.z;
}

// 球体との交差判定
bool BoundingBox::Intersects(const BoundingSphere & sphere, Vector3 * closest) const
{
	// 球体の中心座標に最も近いOBB内の座標を求める
	const Vector3 point = ClosestPoint(sphere.center);
	if (closest != 0)
		*closest = point;
	
	return sphere.Contains(point);
}

// 線分との交差判定
bool BoundingBox::Intersects(const Line & line, Vector3 * intersect) const
{
	float near = 0;
	float far  = line.Length();
	const Vector3 direction = line.Direction();
	
	const bool result = Intersects(line.start, direction, near, far);
	if (result && intersect != 0)
		*intersect = line.start + direction * near;

	return result;
}

// レイとの交差判定
bool BoundingBox::Intersects(const Ray & ray, Vector3 * intersect) const
{
	float near = 0.0f;	// std::numeric_limits<float>::min();
	float far = std::numeric_limits<float>::max();

	const bool result = Intersects(ray.position, ray.direction, near, far);
	if (result && intersect != 0)
		*intersect = ray.position + ray.direction * near;

	return result;
}

// 線分との交差判定
bool BoundingBox::Intersects(const Vector3 & position, const Vector3 & direction, float & near, float & far) const
{
	// xスラブの交差を求める
	if (std::abs(direction.x) > std::numeric_limits<float>::epsilon())
	{
		const float t1 = (min.x - position.x) / direction.x;
		const float t2 = (max.x - position.x) / direction.x;

		if (t1 < t2)
		{
			near = std::max(t1, near);
			far  = std::min(t2, far);
		}
		else
		{
			near = std::max(t2, near);
			far  = std::min(t1, far);
		}

		if (near > far)
			return false;
	}
	else if (position.x < min.x || position.x > max.x)
		return false;

	// yスラブの交差を求める
	if (std::abs(direction.y) > std::numeric_limits<float>::epsilon())
	{
		const float t1 = (min.y - position.y) / direction.y;
		const float t2 = (max.y - position.y) / direction.y;

		if (t1 < t2)
		{
			near = std::max(t1, near);
			far  = std::min(t2, far);
		}
		else
		{
			near = std::max(t2, near);
			far  = std::min(t1, far);
		}

		if (near > far)
			return false;
	}
	else if (position.y < min.y || position.y > max.y)
		return false;

	// zスラブの交差を求める
	if (std::abs(direction.z) > std::numeric_limits<float>::epsilon())
	{
		const float t1 = (min.z - position.z) / direction.z;
		const float t2 = (max.z - position.z) / direction.z;

		if (t1 < t2)
		{
			near = std::max(t1, near);
			far  = std::min(t2, far);
		}
		else
		{
			near = std::max(t2, near);
			far  = std::min(t1, far);
		}

		if (near > far)
			return false;
	}
	else if (position.z < min.z || position.z > max.z)
		return false;

	return near <= far;
}

// 0BBとの交差判定
bool BoundingBox::Intersects(const OrientedBoundingBox & obb) const
{
	return obb.Intersects(*this);
}

// BoundingBoxからターゲットに対して一番近い座標を返す
Vector3 BoundingBox::ClosestPoint(const Vector3 & target) const
{
	return Vector3::Clamp(target, min, max);
}

// 平行移動
BoundingBox BoundingBox::Translate(const Vector3 & position) const
{
	return BoundingBox(min + position, max + position);
}

// コーナーの座標を取得
Vector3 BoundingBox::CornerPoint(int cornerIndex) const
{
	switch (cornerIndex)
	{
	default: // For release builds where assume() is disabled, return always the first option if out-of-bounds.
	case 0:	return min; break;
	case 1:	return Vector3(min.x, min.y, max.z);
	case 2:	return Vector3(min.x, max.y, min.z);
	case 3:	return Vector3(min.x, max.y, max.z);
	case 4:	return Vector3(max.x, min.y, min.z);
	case 5:	return Vector3(max.x, min.y, max.z);
	case 6:	return Vector3(max.x, max.y, min.z);
	case 7:	return max;
	}
}

// サイズの取得
Vector3 BoundingBox::Size() const
{
	return max - min;
}

// 中心座標の取得
Vector3 BoundingBox::Center() const
{
	return (max + min) / 2.0f;
}
