#pragma once

// �V���O���g��
template<class T>
class Singleton
{
public:
	//
	static T * GetInstance()
	{
		static T instance;
		return &instance;
	}

protected:
	// �B���R���X�g���N�^
	Singleton() {}
	// ���z�f�X�g���N�^
	virtual ~Singleton() {}

private:
	void operator=(const Singleton& obj) = delete;	// ������Z�q�֎~
	Singleton(const Singleton &obj) = delete;		// �R�s�[�R���X�g���N�^�֎~
};