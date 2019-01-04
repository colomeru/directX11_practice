#include "OrientedBoundingBox.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Line.h"
#include "Ray.h"
#include "../Math/Matrix.h"
#include "../Math/MathHelper.h"
#include <cmath>

// コンストラクタ
OrientedBoundingBox::OrientedBoundingBox() :
position(0.0f, 0.0f, 0.0f),
extents(1.0f, 1.0f, 1.0f)
{
	axis[0] = Vector3(1.0f, 0.0f, 0.0f);
	axis[1] = Vector3(0.0f, 1.0f, 0.0f);
	axis[2] = Vector3(0.0f, 0.0f, 1.0f);
}

// コンストラクタ
OrientedBoundingBox::OrientedBoundingBox(const Vector3 & position, const Vector3 & extents, const Vector3 & axis_x, const Vector3 & axis_y, const Vector3 & axis_z) :
position(position),
extents(extents)
{
	axis[0] = axis_x;
	axis[1] = axis_y;
	axis[2] = axis_z;
}

// コンストラクタ
OrientedBoundingBox::OrientedBoundingBox(const BoundingBox & aabb) :
position(aabb.Center()),
extents(aabb.Size() / 2.0f)
{
	axis[0] = Vector3(1.0f, 0.0f, 0.0f);
	axis[1] = Vector3(0.0f, 1.0f, 0.0f);
	axis[2] = Vector3(0.0f, 0.0f, 1.0f);
}

// 点が内包されているか？
bool OrientedBoundingBox::Contains(const Vector3 & point) const
{
	const Vector3 pt = point - position;
	return	std::abs(Vector3::Dot(pt, axis[0])) <= extents.x &&
			std::abs(Vector3::Dot(pt, axis[1])) <= extents.y &&
			std::abs(Vector3::Dot(pt, axis[2])) <= extents.z;

}

// 境界ボックスが内包されているか？
bool OrientedBoundingBox::Contains(const BoundingBox & aabb) const
{
	for (int i = 0; i < 8; ++i)
	{
		if (!Contains(aabb.CornerPoint(i))) return false;
	}
	return true;
}

// 境界球が内包されているか？
bool OrientedBoundingBox::Contains(const BoundingSphere & sphere) const
{
	return BoundingBox(-extents, extents).Contains(sphere.Transform(WorldToLocal()));
}

// 線分が内包されているか？
bool OrientedBoundingBox::Contains(const Line & line) const
{
	return Contains(line.start) && Contains(line.end);
}

// 指向性境界ボックスが内包されているか？
bool OrientedBoundingBox::Contains(const OrientedBoundingBox & other) const
{
	for (int i = 0; i < 8; ++i)
	{
		if (!Contains(other.CornerPoint(i))) return false;
	}
	return true;
}

// OBBの交差判定
bool OrientedBoundingBox::Intersects(const OrientedBoundingBox & other, float epsilon) const
{
	Matrix R;
	for (int i = 0; i < 3; ++i)	{
		for (int j = 0; j < 3; ++j)		{
			R.m[i][j] = Vector3::Dot(axis[i], other.axis[j]);
		}
	}
	Matrix AbsR;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			AbsR.m[i][j] = std::abs(R.m[i][j]) + epsilon;
		}
	}
	const float ae[3] = { extents.x, extents.y, extents.z };
	const float be[3] = { other.extents.x, other.extents.y, other.extents.z };
	float ra = 0.0f, rb = 0.0f;
	
	// Test the three major axes of this OBB
	Vector3 tmp = other.position - position;
	float t[3] = { Vector3::Dot(tmp, axis[0]), Vector3::Dot(tmp, axis[1]), Vector3::Dot(tmp, axis[2]) };
	for (int i = 0; i < 3; ++i)
	{
		ra = ae[i];
		rb = be[0] * AbsR.m[i][0] + be[1] * AbsR.m[i][1] + be[2] * AbsR.m[i][2];
		if (std::abs(t[i]) > ra + rb)
			return false;
	}

	// Test the three major axes of this OBB b
	for (int i = 0; i < 3; ++i)
	{
		ra = ae[0] * AbsR.m[0][i] + ae[1] * AbsR.m[1][i] + ae[2] * AbsR.m[2][i];
		rb = be[i];
		if (std::abs(t[0] * R.m[0][i] + t[1] * R.m[1][i] + t[2] * R.m[2][i]) > ra + rb)
			return false;
	}

	// A.x <cross> B.x
	ra = ae[2] * AbsR.m[2][0] + ae[2] * AbsR.m[1][0];
	rb = be[2] * AbsR.m[0][2] + be[2] * AbsR.m[0][1];
	if (std::abs(t[0] * R.m[1][0] - t[1] * R.m[2][0]) > ra + rb)
		return false;

	// A.x <cross> B.y
	ra = ae[2] * AbsR.m[2][1] + ae[2] * AbsR.m[1][1];
	rb = be[0] * AbsR.m[0][2] + be[2] * AbsR.m[0][0];
	if (std::abs(t[2] * R.m[1][1] - t[1] * R.m[2][1]) > ra + rb)
		return false;

	// A.x <cross> B.z
	ra = ae[1] * AbsR.m[2][2] + ae[2] * AbsR.m[1][2];
	rb = be[0] * AbsR.m[0][1] + be[2] * AbsR.m[0][0];
	if (std::abs(t[2] * R.m[1][2] - t[1] * R.m[2][2]) > ra + rb)
		return false;

	// A.y <cross> B.x
	ra = ae[0] * AbsR.m[2][0] + ae[2] * AbsR.m[0][0];
	rb = be[2] * AbsR.m[1][2] + be[2] * AbsR.m[1][1];
	if (std::abs(t[0] * R.m[2][0] - t[2] * R.m[0][0]) > ra + rb)
		return false;

	// A.y <cross> B.y
	ra = ae[0] * AbsR.m[2][1] + ae[2] * AbsR.m[0][1];
	rb = be[0] * AbsR.m[1][2] + be[2] * AbsR.m[1][0];
	if (std::abs(t[0] * R.m[2][1] - t[2] * R.m[0][1]) > ra + rb)
		return false;

	// A.y <cross> B.z
	ra = ae[0] * AbsR.m[2][2] + ae[2] * AbsR.m[0][2];
	rb = be[0] * AbsR.m[1][1] + be[2] * AbsR.m[1][0];
	if (std::abs(t[0] * R.m[2][2] - t[2] * R.m[0][2]) > ra + rb)
		return false;

	// A.z <cross> B.x
	ra = ae[0] * AbsR.m[1][0] + ae[1] * AbsR.m[0][0];
	rb = be[1] * AbsR.m[2][2] + be[2] * AbsR.m[2][1];
	if (std::abs(t[1] * R.m[0][0] - t[0] * R.m[1][0]) > ra + rb)
		return false;

	// A.z <cross> B.y
	ra = ae[0] * AbsR.m[1][1] + ae[1] * AbsR.m[0][1];
	rb = be[0] * AbsR.m[2][2] + be[2] * AbsR.m[2][0];
	if (std::abs(t[1] * R.m[0][1] - t[0] * R.m[1][1]) > ra + rb)
		return false;

	// A.z <cross> B.z
	ra = ae[0] * AbsR.m[1][2] + ae[1] * AbsR.m[0][2];
	rb = be[0] * AbsR.m[2][1] + be[1] * AbsR.m[2][0];
	if (std::abs(t[1] * R.m[0][2] - t[0] * R.m[1][2]) > ra + rb)
		return false;

	// No separating axis exists, so the two OBB don't intersecr.
	return true;
}

// AABBの交差判定
bool OrientedBoundingBox::Intersects(const BoundingBox & aabb, float epsilon) const
{
	return Intersects(OrientedBoundingBox(aabb), epsilon);
}

// 球体の交差判定
bool OrientedBoundingBox::Intersects(const BoundingSphere & sphere, Vector3 * closest) const
{
	Vector3 closest_point = ClosestPoint(sphere.center);
	if (closest)
		*closest = closest_point;

	return sphere.Contains(closest_point);
}

// 線分の交差判定
bool OrientedBoundingBox::Intersects(const Line & line, Vector3 * intersect) const
{
	BoundingBox aabb(-extents, extents);
	const bool result = aabb.Intersects(line.Transform(WorldToLocal()), intersect);
	if (result && intersect)
		*intersect = Vector3::Transform(*intersect, LocalToWorld());

	return result;
}

// レイの交差判定
bool OrientedBoundingBox::Intersects(const Ray & ray, Vector3 * intersect) const
{
	BoundingBox aabb(-extents, extents);
	const bool result = aabb.Intersects(ray.Transform(WorldToLocal()), intersect);
	if (result && intersect)
		*intersect = Vector3::Transform(*intersect, LocalToWorld());
	
	return result;
}

// 線分との交差判定
bool OrientedBoundingBox::Intersects(const Vector3 & position, const Vector3 & direction, float & n, float & f) const
{
	BoundingBox aabb(-extents, extents);
	const Matrix& local = WorldToLocal();
	return aabb.Intersects(Vector3::Transform(position, local), Vector3::TransformNormal(direction, local), n, f);
}

// OBBからターゲットに対して一番近い座標を返す
Vector3 OrientedBoundingBox::ClosestPoint(const Vector3 & target) const
{
	const Vector3& d = target - position;
	Vector3 result = position;
	result += MathHelper::Clamp(Vector3::Dot(d, axis[0]), -extents.x, extents.x) * axis[0];
	result += MathHelper::Clamp(Vector3::Dot(d, axis[1]), -extents.y, extents.y) * axis[1];
	result += MathHelper::Clamp(Vector3::Dot(d, axis[2]), -extents.z, extents.z) * axis[2];

	return result;
}

// 座標変換
OrientedBoundingBox OrientedBoundingBox::Transform(const Matrix & matrix) const
{
	const Vector3 scale = matrix.Scale();
	return OrientedBoundingBox(
		Vector3::Transform(position, matrix),
		Vector3(extents.x * scale.x, extents.y * scale.y, extents.z * scale.z),
		Vector3::Normalize(Vector3::TransformNormal(axis[0], matrix)),
		Vector3::Normalize(Vector3::TransformNormal(axis[1], matrix)),
		Vector3::Normalize(Vector3::TransformNormal(axis[2], matrix)));
}

// ワールド座標に変換する行列を作成
Matrix OrientedBoundingBox::LocalToWorld() const
{
	return Matrix(
		 axis[0].x,  axis[0].y,  axis[0].z, 0.0f,
		 axis[1].x,  axis[1].y,  axis[1].z, 0.0f,
		 axis[2].x,  axis[2].y,  axis[2].z, 0.0f,
		position.x, position.y, position.z, 1.0f
		);
}

// ローカル座標に変換する行列を作成
Matrix OrientedBoundingBox::WorldToLocal() const
{
	return Matrix::InvertFast(LocalToWorld());
}

// コーナーの座標を取得
Vector3 OrientedBoundingBox::CornerPoint(int cornerIndex) const
{
	switch (cornerIndex)
	{
	default:	// For release builds where assume() is disabled, return always the first option if out-of-bounds.
	case 0: return position - extents.x * axis[0] - extents.y * axis[1] - extents.z * axis[2];
	case 1: return position - extents.x * axis[0] - extents.y * axis[1] + extents.z * axis[2];
	case 2: return position - extents.x * axis[0] + extents.y * axis[1] - extents.z * axis[2];
	case 3: return position - extents.x * axis[0] + extents.y * axis[1] + extents.z * axis[2];
	case 4: return position + extents.x * axis[0] - extents.y * axis[1] - extents.z * axis[2];
	case 5: return position + extents.x * axis[0] - extents.y * axis[1] + extents.z * axis[2];
	case 6: return position + extents.x * axis[0] + extents.y * axis[1] - extents.z * axis[2];
	case 7: return position + extents.x * axis[0] + extents.y * axis[1] + extents.z * axis[2];
	}
}

// サイズを取得
Vector3 OrientedBoundingBox::Size() const
{
	return extents * 2.0f;
}

// 中心座標を取得
Vector3 OrientedBoundingBox::Center() const
{
	return position;
}
