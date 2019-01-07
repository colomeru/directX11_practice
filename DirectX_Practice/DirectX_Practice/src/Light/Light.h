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
	// �`����X�V
	void Draw();
	// �ʒu��ݒ�
	void SetPosition(const Vector3& position);
	// �ʒu���擾
	Vector3 GetPosition() const;
	// ������ݒ�
	void SetDirection(const Vector3& direction);
	// �������擾
	Vector3 GetDirection() const;
	// �r���[�ϊ��s����擾
	Matrix GetViewMatrix() const;

protected:
	// �R���X�g���N�^
	Light();
	// �f�X�g���N�^
	virtual ~Light();

private:
	CComPtr<ID3D11Buffer>	m_pConstantBuffer;	// �萔�o�b�t�@
	Vector3					m_Position;			// �����ʒu
	Vector3					m_Direction;		// ��������
	Color					m_Ambient;
	Color					m_Diffuse;
	Color					m_Specular;
	Color					m_Attenuate;
};