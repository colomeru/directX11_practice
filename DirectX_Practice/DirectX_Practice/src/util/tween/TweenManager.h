/*
ColoTween利用メモ
UnityのLeanTweenをモデルに作成

***	関数メモ ***
Update / Remove			: 毎フレーム必ず更新される場所で呼び出す
Clear					: シーンの切り替え時等に呼び出す
Count					: 実行中のTween数
Play					: 登録されているTweenを再生
Stop / StopAll			: 登録されているTweenを停止
SetLoop~				: 次以降Addするtweenのオプション宣言（オプション解除したいときはResetOption）
ResetOption				: オプションをデフォルトに戻す
Add						: Tween追加
Loop					: 無限LoopTween追加			（callback呼び出しは1ループ毎 / 必ず呼び出し側のデストラクタなりで Cancel を呼ぶ）
LoopOnce				: 一度だけのLoopTween追加	（callback呼び出しは1ループ毎）
LoopCount				: 指定数LoopTween追加		（callback呼び出しは1ループ毎）
LoopPingPong			: 特殊LoopTween追加			（callback呼び出しは1ループ毎 / 必ず呼び出し側のデストラクタなりで Cancel を呼ぶ / 回数指定をする場合は先行でSetLoopCountを使う）
Delay					: 遅延関数実行
Cancel / DelayCancel	: 登録されているTweenをキャンセル （成功した場合trueを返す）

*** 引数メモ ***
deltaTime				: フレーム間タイム
value*					: 変動値格納ポインタ
type					: easingタイプ
b						: 開始値（from）
c						: 移動量（to - from）
from*					: 開始値 / 変動値格納ポインタ
to						: 終了値（b + c）
d						: 動作時間
callback				: コールバック
s						: EaseBack時の飛び出る量 s > 1
delay					: 遅延時間
count					: ループ回数
debug*					: 遅延確認 兼 認識ハンドル （値が1で関数実行）

*** 使用メモ ***
callbackにこのクラスのClearを呼ぶものを入れてはいけない
※回避例
if文でフラグを監視しておく
・callback内でフラグを立てる
	↓
・フラグを確認しClearを呼ぶ
	↓
・同処理内でフラグを折る
*/

#pragma once
#include "TweenPtr.h"
#include "TweenObject.h"
#include "EasingUpdateType.h"
#include <functional>
#include <memory>
#include <list>
#include <unordered_map>

struct Vector2;
struct Vector3;

// ツイーンマネージャークラス
class TweenManager
{
public:
	enum EaseType
	{
		Linear,
		EaseInQuad,
		EaseOutQuad,
		EaseInOutQuad,
		EaseInCubic,
		EaseOutCubic,
		EaseInOutCubic,
		EaseInQuart,
		EaseOutQuart,
		EaseInOutQuart,
		EaseInQuint,
		EaseOutQuint,
		EaseInOutQuint,
		EaseInSine,
		EaseOutSine,
		EaseInOutSine,
		EaseInExpo,
		EaseOutExpo,
		EaseInOutExpo,
		EaseInCirc,
		EaseOutCirc,
		EaseInOutCirc,
		EaseInElastic,
		EaseOutElastic,
		EaseInOutElastic,
		EaseInBack,
		EaseOutBack,
		EaseInOutBack,
		EaseInBounce,
		EaseOutBounce,
		EaseInOutBounce
	};

private:
	// Tweenパラメータ構造体
	struct TweenOptionParam
	{
		EasingUpdateType		updateType;		// UpdateType
		int						count;			// LoopCount

		// コンストラクタ
		TweenOptionParam() :
			updateType(EasingUpdateType::Common), count(0)
		{}
		// コンストラクタ
		TweenOptionParam(EasingUpdateType u, int c) :
			updateType(u), count(c)
		{}
	};

private:
	// コンストラクタ
	TweenManager();
	// デストラクタ
	~TweenManager();

	/* コピー禁止 */
	TweenManager(const TweenManager&) = delete;
	TweenManager& operator =(const TweenManager&) = delete;

public:
	static TweenManager &GetInstance() {
		static TweenManager t;
		return t;
	}

	// 初期化
	void Initialize();
	// 更新
	void Update(const float deltaTime);
	// 削除
	void Remove();
	// クリア
	void Clear();
	// 実行中のTween数
	int Count() const;

	/*** 基本 ***/
	// 再生 /* scale : 再生倍率 */
	void Play(float scale = 1.0f);
	// 停止（変動値格納ポインタ指定）
	void Stop(const void* address);
	// 全停止
	void StopAll();

	/*** 追加関連 ***/
	// 追加 /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Add(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f, const TweenOptionParam& param = TweenOptionParam());
	// 追加 /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Add(Vector2* value, const EaseType& type, const Vector2& b, const Vector2& c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f, const TweenOptionParam& param = TweenOptionParam());
	// 追加 /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Add(Vector3* value, const EaseType& type, const Vector3& b, const Vector3& c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f, const TweenOptionParam& param = TweenOptionParam());
	// 追加 /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Add(const EaseType& type, float* from, const float to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// 追加 /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Add(const EaseType& type, Vector2* from, const Vector2 to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// 追加 /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Add(const EaseType& type, Vector3* from, const Vector3& to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);

	/*** ループ関連 ***/
	// ループ /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Loop(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Loop(Vector2* value, const EaseType& type, const Vector2& b, const Vector2 c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Loop(Vector3* value, const EaseType& type, const Vector3& b, const Vector3& c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Loop(const EaseType& type, float* from, const float to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Loop(const EaseType& type, Vector2* from, const Vector2 to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void Loop(const EaseType& type, Vector3* from, const Vector3& to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);	
	// ループ（一度のみ） /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopOnce(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ（一度のみ） /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopOnce(Vector2* value, const EaseType& type, const Vector2& b, const Vector2& c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ（一度のみ） /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopOnce(Vector3* value, const EaseType& type, const Vector3& b, const Vector3& c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ（一度のみ） /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopOnce(const EaseType& type, float* from, const float to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ（一度のみ） /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopOnce(const EaseType& type, Vector2* from, const Vector2 to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ（一度のみ） /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopOnce(const EaseType& type, Vector3* from, const Vector3& to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);	
	// ループ（指定回数） /* *value : 変動値格納ポインタ / type : Easetype / count : loop数 / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopCount(float* value, const EaseType& type, const int count, const float b, const float c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ（指定回数） /* *value : 変動値格納ポインタ / type : Easetype / count : loop数 / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopCount(Vector2* value, const EaseType& type, const int count, const Vector2 b, const Vector2 c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ（指定回数） /* *value : 変動値格納ポインタ / type : Easetype / count : loop数 / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopCount(Vector3* value, const EaseType& type, const int count, const Vector3& b, const Vector3& c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ（指定回数） /* type : Easetype / count : loop数 / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopCount(const EaseType& type, const int count, float* from, const float to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ（指定回数） /* type : Easetype / count : loop数 / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopCount(const EaseType& type, const int count, Vector2* from, const Vector2 to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ（指定回数） /* type : Easetype / count : loop数 / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 */
	void LoopCount(const EaseType& type, const int count, Vector3* from, const Vector3& to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);	
	// 特殊ループ（利用メモ必読） /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 / count : ループ回数 */
	void LoopPingPong(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f, int count = 0);
	// 特殊ループ（利用メモ必読） /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 / count : ループ回数 */
	void LoopPingPong(Vector2* value, const EaseType& type, const Vector2 b, const Vector2 c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f, int count = 0);
	// 特殊ループ（利用メモ必読） /* *value : 変動値格納ポインタ / type : Easetype / b : 開始値 / c : 移動量 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 / count : ループ回数 */
	void LoopPingPong(Vector3* value, const EaseType& type, const Vector3& b, const Vector3& c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f, int count = 0);
	// 特殊ループ（利用メモ必読） /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 / count : ループ回数 */
	void LoopPingPong(const EaseType& type, float* from, const float to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f, int count = 0);
	// 特殊ループ（利用メモ必読） /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 / count : ループ回数 */
	void LoopPingPong(const EaseType& type, Vector2* from, const Vector2 to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f, int count = 0);
	// 特殊ループ（利用メモ必読） /* type : Easetype / *from : 開始値ポインタ / to : 終了値 / d : 終了時間 / callback : 関数 / s : back使用時の補正値 / count : ループ回数 */
	void LoopPingPong(const EaseType& type, Vector3* from, const Vector3& to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f, int count = 0);

	/*** 遅延 ***/
	// 遅延呼び出し /* delay : 遅延時間 / func : 関数 / *debag : 変移値格納ポインタ */
	void Delay(const float delay, const std::function<void()>& func, float* debug = nullptr);

	/*** キャンセル関連 ***/
	// キャンセル /* *value : アドレス */
	bool Cancel(const void* value);
	// キャンセル /* *address : アドレス */
	bool DelayCancel(const void* address = nullptr);

private:
	// tweenlist
	std::list<TweenPtr> tweenList_;
	// easing関数ポインタマップ
	std::unordered_map<EaseType, TweenFuncParam>	easeFuncMap_;
};