#include "Camera.h"
#include "../ConstantBuffer/ConstantBuffer.h"
#include "../DirectX/DirectX11.h"

Camera::Camera() :
	m_Position({ 0.0f, 0.0f, 0.0f }),
	m_Target({ 0.0f, 0.0f, 0.0f}),
	m_Up({ 0.0f, 1.0f, 0.0f }),
	m_Fov(0.0f),
	m_Aspect(0.0f),
	m_NearZ(0.1f),
	m_FarZ(100.0f),
	m_pConstantBuffer(nullptr)
{
	DirectX11::GetInstance()->CreateBuffer(&m_pConstantBuffer, nullptr, sizeof(ViewProjCB), D3D11_BIND_CONSTANT_BUFFER);
}

Camera::~Camera()
{
}

void Camera::Draw()
{
	ViewProjCB cb{
		Matrix::Transpose(GetViewMatrix()),
		Matrix::Transpose(GetProjectionMatrix())
	};
	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);
	DirectX11::GetInstance()->GetContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer.p);
}

void Camera::SetPosition(const Vector3 & position)
{
	m_Position = position;
}

void Camera::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3(x, y, z));
}

Vector3 Camera::GetPosition() const
{
	return m_Position;
}

void Camera::SetTarget(const Vector3 & target)
{
	m_Target = target;
}

void Camera::SetTarget(float x, float y, float z)
{
	SetTarget(Vector3(x, y, z));
}

Vector3 Camera::GetTarget() const
{
	return m_Target;
}

void Camera::SetUp(const Vector3 & up)
{
	m_Up = up;
}

void Camera::SetUp(float x, float y, float z)
{
	SetUp(Vector3(x, y, z));
}

Vector3 Camera::GetUp() const
{
	return m_Up;
}

Matrix Camera::GetViewMatrix() const
{
	return Matrix::CreateLookAt(m_Position, m_Target, m_Up);
}

void Camera::SetFovAngle(float angle)
{
	m_Fov = angle;
}

void Camera::SetAspect(float aspect)
{
	m_Aspect = aspect;
}

void Camera::SetNearFar(float n, float f)
{
	m_NearZ = n;
	m_FarZ  = f;
}

Matrix Camera::GetProjectionMatrix() const
{
	return Matrix::CreatePerspectiveFieldOfView(m_Fov, m_Aspect, m_NearZ, m_FarZ);
}
