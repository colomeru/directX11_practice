#pragma once
#include "EasingUpdateType.h"
#include <functional>
#include <list>

// 関数ポインタ型定義
using TweenFuncParam = std::function<float(float, float, float, float, float)>;

class TweenObject
{
private:
	// Easingパラメータ構造体
	struct EaseParam {
		float*	value;	// 変動値
		float	b;		// 開始値
		float	c;		// 終了値

		// コンストラクタ
		EaseParam(float* value, float b, float c) :
			value(value), b(b), c(c)
		{};
	};

public:
	// コンストラクタ
	TweenObject(void* address, const TweenFuncParam& func, const EasingUpdateType& type, float d = 1.0f, float s = 1.0f, const std::function<void()>& action = nullptr, int loopCount = 0);
	// デストラクタ
	~TweenObject();
	// 更新
	void Update(const float deltaTime);
	// 終了しているか？
	bool IsEnd() const;
	// 終了
	void End();
	// 再生
	void Play(float scale = 1.0f);
	// 停止
	void Stop();

	// 値追加
	void AddValue(float* v, float b, float c);
	// アドレス取得
	void* GetAddress() const;

private:
	// 更新関数取得
	std::function<void(float)> GetFunctionType(const EasingUpdateType& type);
	// 通常更新
	void CommonUpdate(float deltaTime);
	// ループ更新
	void LoopUpdate(float deltaTime);
	// 特殊ループ更新
	void PingPongUpdate(float deltaTime);
	// コールバック呼び出し
	void Invoke();


	/* コピー禁止 */
	TweenObject(const TweenObject&) = delete;
	TweenObject& operator = (const TweenObject&) = delete;

private:
	// ease関数
	std::function<float(float, float, float, float, float)> func_;
	// 更新関数
	std::function<void(float)> updateFunc_;

	// アドレス
	void*					address_;
	// 変動値リスト
	std::list<EaseParam>	values_;

	// 終了時間
	float	d_;
	// Back用
	float	s_;
	// コールバック
	std::function<void()> action_;
	// 時間
	float	timer_;
	// 再生倍率
	float	timeScale_;
	// 終了フラグ
	bool	isEnd_;
	
	// ループ数
	int			loopCount_;
	// 更新時汎用補正値
	float		sine_;
};
