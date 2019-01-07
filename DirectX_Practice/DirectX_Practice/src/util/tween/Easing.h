#pragma once

struct Vector3;

//�C�[�W���O�֐�
class Easing
{
public:
	// �R���X�g���N�^
	Easing() = default;
	// �f�X�g���N�^
	~Easing() = default;

	/* �J�n�_���ړ��ʃp�^�[�� */
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float Linear(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInQuad(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutQuad(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutQuad(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInCubic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutCubic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutCubic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInQuart(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutQuart(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutQuart(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInQuint(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutQuint(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutQuint(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInSine(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutSine(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutSine(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInExpo(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutExpo(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutExpo(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInCirc(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutCirc(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutCirc(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInElastic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutElastic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutElastic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static float EaseInBack(float t, float b, float c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static float EaseOutBack(float t, float b, float c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static float EaseInOutBack(float t, float b, float c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInBounce(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutBounce(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutBounce(float t, float b, float c, float d);

	/* �J�n�_���ړ��ʃp�^�[�� */
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 Linear(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInQuad(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutQuad(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutQuad(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInCubic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutCubic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutCubic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInQuart(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutQuart(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutQuart(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInQuint(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutQuint(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutQuint(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInSine(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutSine(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutSine(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInExpo(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutExpo(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutExpo(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInCirc(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutCirc(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutCirc(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInElastic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutElastic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutElastic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static Vector3 EaseInBack(float t, const Vector3& b, const Vector3& c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static Vector3 EaseOutBack(float t, const Vector3& b, const Vector3& c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static Vector3 EaseInOutBack(float t, const Vector3& b, const Vector3& c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInBounce(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutBounce(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutBounce(float t, const Vector3& b, const Vector3& c, float d);
};