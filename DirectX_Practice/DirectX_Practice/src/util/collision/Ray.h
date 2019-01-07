#pragma once
#include "../Math/Vector3.h"

struct Matrix;

// レイ
struct Ray
{
	Vector3 position;		// 座標
	Vector3 direction;		// 向き

	// コンストラクタ
	Ray();
	// コンストラクタ
	Ray(const Vector3& position, const Vector3& direction);
	// 座標変換
	Ray Transform(const Matrix& matrix) const;
};
