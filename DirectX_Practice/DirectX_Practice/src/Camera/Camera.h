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
	// �ʒu��ݒ�
	void SetPosition(const Vector3& position);
	// �ʒu��ݒ�
	void SetPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	// �ʒu���擾
	Vector3 GetPosition() const;
	// ������ݒ�
	void SetTarget(const Vector3& target);
	// ������ݒ�
	void SetTarget(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	// �������擾
	Vector3 GetTarget() const;
	// �������ݒ�
	void SetUp(const Vector3& up);
	// �������ݒ�
	void SetUp(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	// ��������擾
	Vector3 GetUp() const;
	// �r���[�ϊ��s����擾
	Matrix GetViewMatrix() const;

	// �J�����̎���p��ݒ�
	void SetFovAngle(float angle);
	// �J�����̃A�X�y�N�g���ݒ�
	void SetAspect(float aspect);
	// �J�����̃N���b�s���O������ݒ�
	void SetNearFar(float n, float f);
	// �����ϊ��s����擾
	Matrix GetProjectionMatrix() const;

protected:
	// �R���X�g���N�^
	Camera();
	// �f�X�g���N�^
	virtual ~Camera();

private:
	Vector3			m_Position;		// �J�������W
	Vector3			m_Target;		// �J�����^�[�Q�b�g
	Vector3			m_Up;			// �J���������

	float			m_Fov;			// ����p
	float			m_Aspect;		// �A�X�y�N�g��
	float			m_NearZ;		// �ŋߕ`�拗��
	float			m_FarZ;			// �ŉ��`�拗��
};
