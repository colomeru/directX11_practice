#pragma once
#include "../Math/Vector3.h"

// 線分
struct Line
{
	Vector3 start;		// 始点
	Vector3 end;		// 終点

	// コンストラクタ
	Line();
	// コンストラクタ
	Line(const Vector3& start, const Vector3& end);
	// 長さを求める
	float Length() const;
	// 始点から終点への方向を返す
	Vector3 Direction() const;
	// 座標変換
	Line Transform(const Matrix& matrix) const;
};
