#ifndef MATRIX_H_
#define MATRIX_H_

struct Vector3;
struct Quaternion;

struct Matrix {
	float m[4][4];

	// �R���X�g���N�^
	Matrix(
		float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f, float m14 = 0.0f,
		float m21 = 0.0f, float m22 = 1.0f, float m23 = 0.0f, float m24 = 0.0f,
		float m31 = 0.0f, float m32 = 0.0f, float m33 = 1.0f, float m34 = 0.0f,
		float m41 = 0.0f, float m42 = 0.0f, float m43 = 0.0f, float m44 = 1.0f);

	// �X�P�[�����O Matrix ���쐬���܂��B
	static Matrix CreateScale(const Vector3& scale);
	// �ړ� Matrix ���쐬���܂��B
	static Matrix CreateTranslation(const Vector3& translation);
	// �C�ӂ̃x�N�g���̎��͂���]����V���� Matrix ���쐬���܂�
	static Matrix CreateFromAxisAngle(const Vector3& axis, float degree);
	// Quaternion �����] Matrix ���쐬���܂�
	static Matrix CreateFromQuaternion(const Quaternion& rotate);
	// �w�肵�����[�A�s�b�`�A���[���ōs����쐬���܂�
	static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);
	// x ������]���Ƃ��Ē��_�̃Z�b�g����]�����邽�߂Ɏg�p�ł���s����쐬���܂�
	static Matrix CreateRotationX(float degree);
	// y ������]���Ƃ��Ē��_�̃Z�b�g����]�����邽�߂Ɏg�p�ł���s����쐬���܂�
	static Matrix CreateRotationY(float degree);
	// z ������]���Ƃ��Ē��_�̃Z�b�g����]�����邽�߂Ɏg�p�ł���s����쐬���܂�
	static Matrix CreateRotationZ(float degree);
	// ���[���h�s����쐬���܂��B
	static Matrix CreateWorld(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
	// ���[���h�s����쐬���܂��B
	static Matrix CreateWorld(const Vector3& scale, const Matrix& rotate, const Vector3& translation);
	// ���[���h�s����쐬���܂��B
	static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up);
	// ����Ɋ�Â��āA�p�[�X�y�N�e�B�u�ˉe�s����쐬���܂�
	static Matrix CreatePerspectiveFieldOfView(float fov, float aspect, float near, float far);
	// �r���[�s����쐬���܂�
	static Matrix CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up);
	// �X�N���[���s����쐬���܂�
	static Matrix CreateScreen(float x, float y, float width, float height);

	// �t�s����v�Z���܂��B
	static Matrix Invert(const Matrix& matrix);
	// 
	static Matrix InvertFast(const Matrix& matrix);
	// �s��̍s�Ɨ�����ւ��܂��B
	static Matrix Transpose(const Matrix& matrix);
	// 2�̍s��̑Ή�����l�̊Ԃ���`��Ԃ��܂��B
	static Matrix Lerp(const Matrix& value1, const Matrix& value2, float amount);

	// 3D �X�P�[�� / ��] / �ϊ�(SRT) Matrix ����X�J���[�A�ϊ��A��]�̃R���|�[�l���g���擾���܂�
	void Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) const;
	// ��]�s��𐳋K�����܂��B
	static Matrix NormalizeRotationMatrix(const Matrix& matrix);
	// ��]�s��𐳋K�����܂��B
	Matrix& NormalizeRotationMatrix();

	// �O���x�N�g�����擾
	Vector3 Forward() const;
	// �O���x�N�g����ݒ�
	Matrix& Forward(const Vector3& forward);
	// ����x�N�g�����擾
	Vector3 Backward() const;
	// ����x�N�g����ݒ�
	Matrix& Backward(const Vector3& backward);
	// ����x�N�g�����擾
	Vector3 Up() const;
	// ����x�N�g����ݒ�
	Matrix& Up(const Vector3& up);
	// �����x�N�g�����擾
	Vector3 Down() const;
	// �����x�N�g����ݒ�
	Matrix& Down(const Vector3& down);
	// ���x�N�g�����擾
	Vector3 Left() const;
	// ���x�N�g����ݒ�
	Matrix& Left(const Vector3& left);
	// �E�x�N�g�����擾
	Vector3 Right() const;
	// �E�x�N�g����ݒ�
	Matrix& Right(const Vector3& rignt);
	// ���s�ړ��x�N�g�����擾�܂��͐ݒ肵�܂��B
	Vector3 Translation() const;
	// ���s�ړ��x�N�g�����擾�܂��͐ݒ肵�܂��B
	Matrix& Translation(const Vector3& translation);
	// �X�P�[���l���擾
	Vector3 Scale() const;
	// ��]�̎擾
	Quaternion Rotation() const;
	// ��]�s��̎擾
	Matrix RotationMatrix() const;

	// �P�ʍs��
	static const Matrix Identity;
	// �[���s��
	static const Matrix Zero;

#ifdef __DXLIB
	// Dxlib�p�ϊ��֐�
	explicit Matrix(const DxLib::MATRIX& mat) {
		memcpy(this, &mat, sizeof(Matrix));
	}
	operator const DxLib::MATRIX() const {
		DxLib::MATRIX result;
		memcpy(&result, this, sizeof(DxLib::MATRIX));
		return result;
	}
#endif
};

// �P�����Z�q�I�[�o�[���[�h
Matrix operator - (const Matrix& m);

// ������Z�q�I�[�o�[���[�h
Matrix& operator += (Matrix& m1, const Matrix& m2);
Matrix& operator -= (Matrix& m1, const Matrix& m2);
Matrix& operator *= (Matrix& m1, const Matrix& m2);
Matrix& operator *= (Matrix& m, float s);
Matrix& operator /= (Matrix& m, float s);

// �Q�����Z�q�I�[�o�[���[�h
Matrix  operator * (Matrix m1, const Matrix& m2);
Matrix  operator * (Matrix m, float s);
Matrix  operator * (Matrix s, float m);
Vector3 operator * (const Vector3& v, const Matrix& m);
Vector3 operator * (const Matrix& m, const Vector3& v);

// �Q�����Z�q�I�[�o�[���[�h
Matrix operator + (Matrix m1, const Matrix& m2);
Matrix operator - (Matrix m1, const Matrix& m2);
Matrix operator * (Matrix m, float s);
Matrix operator * (float s, Matrix m);
Matrix operator / (Matrix m, float s);

#endif
