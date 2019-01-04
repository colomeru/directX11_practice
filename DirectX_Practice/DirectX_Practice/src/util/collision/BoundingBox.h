#pragma once
#include "../Math/Vector3.h"

struct BoundingSphere;
struct Line;
struct Ray;
struct OrientedBoundingBox;

// 境界ボックス（AABB）
struct BoundingBox
{
	Vector3		min;	// 境界中に含まれる最小座標
	Vector3		max;	// 境界中に含まれる最大座標
	
	// コンストラクタ
	BoundingBox();
	// コンストラクタ
	BoundingBox(const Vector3& min, const Vector3& max);

	// 点が内包されているか？
	bool Contains(const Vector3& point) const;
	// 境界ボックスが内包されているか？
	bool Contains(const BoundingBox& other) const;
	// 境界球が内包されているか？
	bool Contains(const BoundingSphere& sphere) const;
	// 線分が内包されているか？
	bool Contains(const Line& line) const;
	// OBBが内包されているか？
	bool Contains(const OrientedBoundingBox& obb) const;

	// 境界ボックスとの交差判定
	bool Intersects(const BoundingBox& other) const;
	// 球体との交差判定
	bool Intersects(const BoundingSphere& sphere, Vector3* closest = 0) const;
	// 線分との交差判定
	bool Intersects(const Line& line, Vector3* intersect = 0) const;
	// レイとの交差判定
	bool Intersects(const Ray& ray, Vector3* intersect = 0) const;
	// 線分との交差判定
	bool Intersects(const Vector3& position, const Vector3& direction, float &near, float &far) const;
	// 0BBとの交差判定
	bool Intersects(const OrientedBoundingBox& obb) const;

	// BoundingBoxからターゲットに対して一番近い座標を返す
	Vector3 ClosestPoint(const Vector3& target) const;

	// 平行移動
	BoundingBox Translate(const Vector3& position) const;
	// コーナーの座標を取得
	Vector3 CornerPoint(int cornerIndex) const;
	// サイズの取得
	Vector3 Size() const;
	// 中心座標の取得
	Vector3 Center() const;
};
