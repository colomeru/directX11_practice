#pragma once
#include "../util/Singleton.h"
#include "../util/math/Vector3.h"
#include "../util/math/Matrix.h"

class Camera : public Singleton<Camera>
{
public:
	//
	friend class Singleton<Camera>;

public:
	// 位置を設定
	void SetPosition(const Vector3& position);
	// 位置を設定
	void SetPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	// 位置を取得
	Vector3 GetPosition() const;
	// 視線を設定
	void SetTarget(const Vector3& target);
	// 視線を設定
	void SetTarget(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	// 視線を取得
	Vector3 GetTarget() const;
	// 上方向を設定
	void SetUp(const Vector3& up);
	// 上方向を設定
	void SetUp(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	// 上方向を取得
	Vector3 GetUp() const;
	// ビュー変換行列を取得
	Matrix GetViewMatrix() const;

	// カメラの視野角を設定
	void SetFovAngle(float angle);
	// カメラのアスペクト比を設定
	void SetAspect(float aspect);
	// カメラのクリッピング距離を設定
	void SetNearFar(float n, float f);
	// 透視変換行列を取得
	Matrix GetProjectionMatrix() const;

protected:
	// コンストラクタ
	Camera();
	// デストラクタ
	virtual ~Camera();

private:
	Vector3			m_Position;		// カメラ座標
	Vector3			m_Target;		// カメラターゲット
	Vector3			m_Up;			// カメラ上向き

	float			m_Fov;			// 視野角
	float			m_Aspect;		// アスペクト比
	float			m_NearZ;		// 最近描画距離
	float			m_FarZ;			// 最遠描画距離
};
