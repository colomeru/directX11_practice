#include "TPSCamera.h"
#include "Camera.h"
#include "../Input/Keyboard.h"
#include "../util/math/Vector3.h"

TPSCamera::TPSCamera()
{
	m_posture = Matrix::CreateRotationY(180.0f)
			  * Matrix::CreateTranslation(Vector3(0.0f, 10.0f, -50.0f));

	Camera::GetInstance()->SetFovAngle(60.0f);
	Camera::GetInstance()->SetAspect(1280.0f / 720.0f);
	Camera::GetInstance()->SetNearFar(1.0f, 1000.0f);
}

TPSCamera::~TPSCamera()
{
}

void TPSCamera::Update()
{
	// 回転
	float yaw, pitch, roll;
	yaw = pitch = roll = 0.0f;
	if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_A))
		yaw -= 1.0f;
	else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_D))
		yaw += 1.0f;
	if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_W))
		pitch += 1.0f;
	else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_S))
		pitch -= 1.0f;
	m_posture *= Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);

	// 移動
	float speed = 0.5f;
	if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_LEFT))
		Translation(m_posture.Left() * speed);
	else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_RIGHT))
		Translation(m_posture.Right() * speed);
	if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_PAGEUP))
		Translation(m_posture.Up() * speed);
	else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_PAGEDOWN))
		Translation(m_posture.Down() * speed);
	if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_UP))
		Translation(m_posture.Forward() * speed);
	else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_DOWN))
		Translation(m_posture.Backward() * speed);

	// カメラの前方をターゲットとする
	auto target = m_posture.Translation() + m_posture.Forward();

	Camera::GetInstance()->SetPosition(m_posture.Translation());
	Camera::GetInstance()->SetTarget(target);
	Camera::GetInstance()->SetUp(m_posture.Up());
}

void TPSCamera::Draw() const
{
	Camera::GetInstance()->SetShader();
}

void TPSCamera::Translation(const Vector3 & v)
{
	m_posture *= Matrix::CreateTranslation(v);
}
