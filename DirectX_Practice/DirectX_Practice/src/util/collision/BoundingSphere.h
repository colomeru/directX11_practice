#pragma once
#include "../Math/Vector3.h"

// 境界球
struct BoundingSphere
{
	Vector3 center;		// 中心の座標
	float	radius;		// 半径

	// コンストラクタ
	BoundingSphere();
	// コンストラクタ
	BoundingSphere(const Vector3& center, float radius);
	// 座標が含まれているかどうか？
	bool Contains(const Vector3& target) const;
	// 境界球が含まれているか？
	bool Contains(const BoundingSphere& other) const;
	// 交差しているか？
	bool Intersects(const BoundingSphere& other) const;
	// 座標変換
	BoundingSphere Transform(const Matrix& matrix) const;
};