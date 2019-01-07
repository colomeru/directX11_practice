#include "Line.h"

// コンストラクタ
Line::Line() :
start(0.0f, 0.0f, 0.0f),
end(0.0f, 0.0f, 0.0f)
{
}

// コンストラクタ
Line::Line(const Vector3 & start, const Vector3 & end) :
start(start),
end(end)
{
}

// 長さを求める
float Line::Length() const
{
	return Vector3::Distance(start, end);
}

// 始点から終点への方向を返す
Vector3 Line::Direction() const
{
	return Vector3::Normalize(end - start);
}

// 座標変換
Line Line::Transform(const Matrix & matrix) const
{
	return Line(Vector3::Transform(start, matrix), Vector3::Transform(end, matrix));
}
