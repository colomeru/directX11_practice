#include "Light.h"
#include "../DirectX/DirectX11.h"
#include "../ConstantBuffer/ConstantBuffer.h"
#include "../Camera/Camera.h"

Light::Light() :
	m_pConstantBuffer(nullptr),
	m_Position({ 0.0f, 0.0f, 0.0f }),
	m_Direction({ 0.0f, 0.0f, 0.0f }),
	m_Ambient(0.2f, 0.2f, 0.2f, 1.0f),
	m_Diffuse(1.0f, 1.0f, 1.0f, 0.0f),
	m_Specular(1.0f, 1.0f, 1.0f, 0.0f),
	m_Attenuate(1.0f, 0.0f, 0.0f, 0.0f)
{
	DirectX11::GetInstance()->CreateBuffer(&m_pConstantBuffer, nullptr, sizeof(LightCB), D3D11_BIND_CONSTANT_BUFFER);
}

Light::~Light()
{
}

void Light::Draw()
{
	LightCB clb;
	clb.eyePosition			= Camera::GetInstance()->GetPosition();
	clb.ambient				= m_Ambient;
	clb.pntLight.position	= GetPosition();
	clb.pntLight.diffuse	= m_Diffuse;
	clb.pntLight.specular	= m_Specular;
	clb.pntLight.attenuate	= m_Attenuate;

	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, NULL, &clb, 0, 0);
	DirectX11::GetInstance()->GetContext()->PSSetConstantBuffers(1, 1, &m_pConstantBuffer.p);
}

void Light::SetPosition(const Vector3& position)
{
	m_Position = position;
}

Vector3 Light::GetPosition() const
{
	return m_Position;
}

void Light::SetDirection(const Vector3& direction)
{
	m_Direction = direction;
}

Vector3 Light::GetDirection() const
{
	return m_Direction;
}

Matrix Light::GetViewMatrix() const
{
	// フォーカス仮データ
	// 中心やや上を見てるとする
	//Vector3 focus = Vector3(0.0f, 10.0f, 0.0f);
	Vector3 focus = m_Position + m_Direction;

	return Matrix::CreateLookAt(m_Position, focus.Normalize(), Vector3::Up);
}