#pragma once

// シングルトン
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
	// 隠しコンストラクタ
	Singleton() {}
	// 仮想デストラクタ
	virtual ~Singleton() {}

private:
	void operator=(const Singleton& obj) = delete;	// 代入演算子禁止
	Singleton(const Singleton &obj) = delete;		// コピーコンストラクタ禁止
};