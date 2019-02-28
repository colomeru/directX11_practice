#pragma once
#include <d3d11.h>
#include <atlbase.h>
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
	CComPtr<ID3D11Buffer>	m_pConstantBuffer;	// コンスタントバッファ
	Matrix					m_pose;				// 姿勢
};
