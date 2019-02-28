#include "FreeCamera.h"
#include "Camera.h"
#include "../Input/Keyboard.h"
#include "../util/math/Vector3.h"
#include "../Util.h"
#include "../ConstantBuffer/ConstantBuffer.h"
#include "../DirectX/DirectX11.h"

FreeCamera::FreeCamera() :
	m_pose(Matrix::CreateRotationY(180.0f) * Matrix::CreateTranslation(Vector3(0.0f, 10.0f, -50.0f)))
{
	DirectX11::GetInstance()->CreateBuffer(&m_pConstantBuffer, nullptr, sizeof(ViewProjCB), D3D11_BIND_CONSTANT_BUFFER);

	Camera::GetInstance()->SetFovAngle(60.0f);
	Camera::GetInstance()->SetAspect((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT);
	Camera::GetInstance()->SetNearFar(0.1f, 500.0f);
}

FreeCamera::~FreeCamera()
{
}

void FreeCamera::Update()
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
	m_pose *= Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);

	// 移動
	float speed = 0.3f;
	if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_LEFT))
		Translation(m_pose.Left() * speed);
	else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_RIGHT))
		Translation(m_pose.Right() * speed);
	if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_PAGEUP))
		Translation(m_pose.Up() * speed);
	else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_PAGEDOWN))
		Translation(m_pose.Down() * speed);
	if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_UP))
		Translation(m_pose.Forward() * speed);
	else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_DOWN))
		Translation(m_pose.Backward() * speed);

	// カメラの前方をターゲットとする
	auto target = m_pose.Translation() + m_pose.Forward();

	Camera::GetInstance()->SetPosition(m_pose.Translation());
	Camera::GetInstance()->SetTarget(target);
	Camera::GetInstance()->SetUp(m_pose.Up());
}

void FreeCamera::Draw() const
{
	ViewProjCB cb{
		Matrix::Transpose(Camera::GetInstance()->GetViewMatrix()),
		Matrix::Transpose(Camera::GetInstance()->GetProjectionMatrix())
	};
	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);
	DirectX11::GetInstance()->GetContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer.p);
}

void FreeCamera::Translation(const Vector3 & v)
{
	m_pose *= Matrix::CreateTranslation(v);
}
