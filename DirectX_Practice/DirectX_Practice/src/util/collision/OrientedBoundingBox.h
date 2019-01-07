#pragma once
#include "../Math/Vector3.h"

struct BoundingSphere;
struct BoundingBox;
struct Line;
struct Ray;
struct Matrix;

// 指向性境界ボックス（OBB）
struct OrientedBoundingBox
{
	Vector3		position;	// 中心の座標
	Vector3		extents;	// 各軸のサイズ
	Vector3		axis[3];	// 各軸の向き
	
	// コンストラクタ
	OrientedBoundingBox();
	// コンストラクタ
	OrientedBoundingBox(const Vector3& position, const Vector3& extents,
						const Vector3& axis_x, const Vector3& axis_y, const Vector3& axis_z);
	// コンストラクタ
	explicit OrientedBoundingBox(const BoundingBox& aabb);

	// 点が内包されているか？
	bool Contains(const Vector3& point) const;
	// 境界ボックスが内包されているか？
	bool Contains(const BoundingBox& aabb) const;
	// 境界球が内包されているか？
	bool Contains(const BoundingSphere& sphere) const;
	// 線分が内包されているか？
	bool Contains(const Line& line) const;
	// 指向性境界ボックスが内包されているか？
	bool Contains(const OrientedBoundingBox& other) const;

	// OBBの交差判定
	bool Intersects(const OrientedBoundingBox& other, float epsilon = 1e-3f) const;
	// AABBの交差判定
	bool Intersects(const BoundingBox& aabb, float epsilon = 1e-3f) const;
	// 球体の交差判定
	bool Intersects(const BoundingSphere& sphere, Vector3* closest = 0) const;
	// 線分の交差判定
	bool Intersects(const Line& line, Vector3* intersect = 0) const;
	// レイの交差判定
	bool Intersects(const Ray& ray, Vector3* intersect = 0) const;
	// 線分との交差判定
	bool Intersects(const Vector3& position, const Vector3& direction, float &near, float &far) const;

	// OBBからターゲットに対して一番近い座標を返す
	Vector3 ClosestPoint(const Vector3& target) const;

	// 座標変換
	OrientedBoundingBox Transform(const Matrix& matrix) const;
	// ワールド座標に変換する行列を作成
	Matrix LocalToWorld() const;
	// ローカル座標に変換する行列を作成
	Matrix WorldToLocal() const;

	// コーナーの座標を取得
	Vector3 CornerPoint(int cornerIndex) const;
	// サイズを取得
	Vector3 Size() const;
	// 中心座標を取得
	Vector3 Center() const;
};