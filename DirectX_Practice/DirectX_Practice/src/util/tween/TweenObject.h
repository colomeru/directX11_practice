#pragma once
#include "EasingUpdateType.h"
#include <functional>
#include <list>

// �֐��|�C���^�^��`
using TweenFuncParam = std::function<float(float, float, float, float, float)>;

class TweenObject
{
private:
	// Easing�p�����[�^�\����
	struct EaseParam {
		float*	value;	// �ϓ��l
		float	b;		// �J�n�l
		float	c;		// �I���l

		// �R���X�g���N�^
		EaseParam(float* value, float b, float c) :
			value(value), b(b), c(c)
		{};
	};

public:
	// �R���X�g���N�^
	TweenObject(void* address, const TweenFuncParam& func, const EasingUpdateType& type, float d = 1.0f, float s = 1.0f, const std::function<void()>& action = nullptr, int loopCount = 0);
	// �f�X�g���N�^
	~TweenObject();
	// �X�V
	void Update(const float deltaTime);
	// �I�����Ă��邩�H
	bool IsEnd() const;
	// �I��
	void End();
	// �Đ�
	void Play(float scale = 1.0f);
	// ��~
	void Stop();

	// �l�ǉ�
	void AddValue(float* v, float b, float c);
	// �A�h���X�擾
	void* GetAddress() const;

private:
	// �X�V�֐��擾
	std::function<void(float)> GetFunctionType(const EasingUpdateType& type);
	// �ʏ�X�V
	void CommonUpdate(float deltaTime);
	// ���[�v�X�V
	void LoopUpdate(float deltaTime);
	// ���ꃋ�[�v�X�V
	void PingPongUpdate(float deltaTime);
	// �R�[���o�b�N�Ăяo��
	void Invoke();


	/* �R�s�[�֎~ */
	TweenObject(const TweenObject&) = delete;
	TweenObject& operator = (const TweenObject&) = delete;

private:
	// ease�֐�
	std::function<float(float, float, float, float, float)> func_;
	// �X�V�֐�
	std::function<void(float)> updateFunc_;

	// �A�h���X
	void*					address_;
	// �ϓ��l���X�g
	std::list<EaseParam>	values_;

	// �I������
	float	d_;
	// Back�p
	float	s_;
	// �R�[���o�b�N
	std::function<void()> action_;
	// ����
	float	timer_;
	// �Đ��{��
	float	timeScale_;
	// �I���t���O
	bool	isEnd_;
	
	// ���[�v��
	int			loopCount_;
	// �X�V���ėp�␳�l
	float		sine_;
};
