#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include "../util/Singleton.h"
#include "../util/math/Vector3.h"
#include "../util/math/Matrix.h"
#include "../util/math/Color.h"

class Light : public Singleton<Light>
{
public:
	//
	friend class Singleton<Light>;

public:
	// 描画情報更新
	void Draw();
	// 位置を設定
	void SetPosition(const Vector3& position);
	// 位置を取得
	Vector3 GetPosition() const;
	// 方向を設定
	void SetDirection(const Vector3& direction);
	// 方向を取得
	Vector3 GetDirection() const;
	// ビュー変換行列を取得
	Matrix GetViewMatrix() const;

protected:
	// コンストラクタ
	Light();
	// デストラクタ
	virtual ~Light();

private:
	CComPtr<ID3D11Buffer>	m_pConstantBuffer;	// 定数バッファ
	Vector3					m_Position;			// 光源位置
	Vector3					m_Direction;		// 光源方向
	Color					m_Ambient;
	Color					m_Diffuse;
	Color					m_Specular;
	Color					m_Attenuate;
};