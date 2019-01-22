#pragma once
#include "../util/math/Matrix.h"

struct Vector3;

class FreeCamera
{
public:
	// コンストラクタ
	FreeCamera();
	// デストラクタ
	~FreeCamera();
	// 更新
	void Update();
	// 描画
	void Draw() const;

	// 平行移動
	void Translation(const Vector3& v);

private:
	Matrix		m_posture;	// 姿勢
};
