#include "TweenManager.h"
#include "Easing.h"
#include "../math/MathHelper.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"

TweenManager::TweenManager()
{
	Clear();
	easeFuncMap_.clear();

	// マップに関数を登録
	easeFuncMap_[EaseType::Linear]			= [](float t, float b, float c, float d, float s) { return Easing::Linear(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInQuad]		= [](float t, float b, float c, float d, float s) { return Easing::EaseInQuad(t, b, c, d); };
	easeFuncMap_[EaseType::EaseOutQuad]		= [](float t, float b, float c, float d, float s) { return Easing::EaseOutQuad(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInOutQuad]	= [](float t, float b, float c, float d, float s) { return Easing::EaseInOutQuad(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInCubic]		= [](float t, float b, float c, float d, float s) { return Easing::EaseInCubic(t, b, c, d); };
	easeFuncMap_[EaseType::EaseOutCubic]	= [](float t, float b, float c, float d, float s) { return Easing::EaseOutCubic(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInOutCubic]	= [](float t, float b, float c, float d, float s) { return Easing::EaseInOutCubic(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInQuart]		= [](float t, float b, float c, float d, float s) { return Easing::EaseInQuart(t, b, c, d); };
	easeFuncMap_[EaseType::EaseOutQuart]	= [](float t, float b, float c, float d, float s) { return Easing::EaseOutQuart(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInOutQuart]	= [](float t, float b, float c, float d, float s) { return Easing::EaseInOutQuart(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInQuint]		= [](float t, float b, float c, float d, float s) { return Easing::EaseInQuint(t, b, c, d); };
	easeFuncMap_[EaseType::EaseOutQuint]	= [](float t, float b, float c, float d, float s) { return Easing::EaseOutQuint(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInOutQuint]	= [](float t, float b, float c, float d, float s) { return Easing::EaseInOutQuint(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInSine]		= [](float t, float b, float c, float d, float s) { return Easing::EaseInSine(t, b, c, d); };
	easeFuncMap_[EaseType::EaseOutSine]		= [](float t, float b, float c, float d, float s) { return Easing::EaseOutSine(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInOutSine]	= [](float t, float b, float c, float d, float s) { return Easing::EaseInOutSine(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInExpo]		= [](float t, float b, float c, float d, float s) { return Easing::EaseInExpo(t, b, c, d); };
	easeFuncMap_[EaseType::EaseOutExpo]		= [](float t, float b, float c, float d, float s) { return Easing::EaseOutExpo(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInOutExpo]	= [](float t, float b, float c, float d, float s) { return Easing::EaseInOutExpo(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInCirc]		= [](float t, float b, float c, float d, float s) { return Easing::EaseInCirc(t, b, c, d); };
	easeFuncMap_[EaseType::EaseOutCirc]		= [](float t, float b, float c, float d, float s) { return Easing::EaseOutCirc(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInOutCirc]	= [](float t, float b, float c, float d, float s) { return Easing::EaseInOutCirc(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInElastic]		= [](float t, float b, float c, float d, float s) { return Easing::EaseInElastic(t, b, c, d); };
	easeFuncMap_[EaseType::EaseOutElastic]		= [](float t, float b, float c, float d, float s) { return Easing::EaseOutElastic(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInOutElastic]	= [](float t, float b, float c, float d, float s) { return Easing::EaseInOutElastic(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInBack]		= [](float t, float b, float c, float d, float s) { return Easing::EaseInBack(t, b, c, d, s); };
	easeFuncMap_[EaseType::EaseOutBack]		= [](float t, float b, float c, float d, float s) { return Easing::EaseOutBack(t, b, c, d, s); };
	easeFuncMap_[EaseType::EaseInOutBack]	= [](float t, float b, float c, float d, float s) { return Easing::EaseInOutBack(t, b, c, d, s); };
	easeFuncMap_[EaseType::EaseInBounce]	= [](float t, float b, float c, float d, float s) { return Easing::EaseInBounce(t, b, c, d); };
	easeFuncMap_[EaseType::EaseOutBounce]	= [](float t, float b, float c, float d, float s) { return Easing::EaseOutBounce(t, b, c, d); };
	easeFuncMap_[EaseType::EaseInOutBounce] = [](float t, float b, float c, float d, float s) { return Easing::EaseInOutBounce(t, b, c, d); };
}

TweenManager::~TweenManager()
{
}

void TweenManager::Initialize()
{
	Clear();
}

void TweenManager::Update(const float deltaTime)
{
	// 更新
	for (auto& tween : tweenList_)
		tween->Update(deltaTime);
}

void TweenManager::Remove()
{
	tweenList_.remove_if([](TweenPtr ease)
	{
		return ease->IsEnd();
	});
}

void TweenManager::Clear()
{
	tweenList_.clear();
}

int TweenManager::Count() const
{
	return static_cast<int>(tweenList_.size());
}

void TweenManager::Play(float scale)
{
	for (auto& ease : tweenList_)
		ease->Play(scale);
}

void TweenManager::Stop(const void * address)
{
	for (auto& tween : tweenList_)
	{
		if (tween->GetAddress() == address)
		{ 
			tween->Stop();
			return;
		}
	}
}

void TweenManager::StopAll()
{
	for (auto& ease : tweenList_)
		ease->Stop();
}

void TweenManager::Add(float* value, const EaseType& type, const float b, const float c, const float d, const std::function<void()>& action, const float s, const TweenOptionParam& param)
{
	auto address = (void*)value;

	for (auto& t : tweenList_)
	{
		if (address == t->GetAddress())
			t->End();
	}

	auto tween = std::make_shared<TweenObject>(address, easeFuncMap_[type], param.updateType, d, s, action, param.count);
	tween->AddValue(value, b, c);
	tweenList_.push_back(tween);
}

void TweenManager::Add(Vector2 * value, const EaseType & type, const Vector2 & b, const Vector2 & c, const float d, const std::function<void()>& action, const float s, const TweenOptionParam& param)
{
	auto address = (void*)value;

	for (auto& t : tweenList_)
	{
		if (address == t->GetAddress())
			t->End();
	}

	auto tween = std::make_shared<TweenObject>(address, easeFuncMap_[type], param.updateType, d, s, action, param.count);
	tween->AddValue(&value->x, b.x, c.x);
	tween->AddValue(&value->y, b.y, c.y);
	tweenList_.push_back(tween);
}

void TweenManager::Add(Vector3 * value, const EaseType & type, const Vector3 & b, const Vector3 & c, const float d, const std::function<void()>& action, const float s, const TweenOptionParam& param)
{
	auto address = (void*)value;

	for (auto& t : tweenList_)
	{
		if (address == t->GetAddress())
			t->End();
	}

	auto tween = std::make_shared<TweenObject>(address, easeFuncMap_[type], param.updateType, d, s, action, param.count);
	tween->AddValue(&value->x, b.x, c.x);
	tween->AddValue(&value->y, b.y, c.y);
	tween->AddValue(&value->z, b.z, c.z);
	tweenList_.push_back(tween);
}

void TweenManager::Add(const EaseType & type, float * from, const float to, const float d, const std::function<void()>& callback, const float s)
{
	Add(from, type, *from, to - *from, d, callback, s);
}

void TweenManager::Add(const EaseType & type, Vector2 * from, const Vector2 to, const float d, const std::function<void()>& callback, const float s)
{
	Add(from, type, *from, to - *from, d, callback, s);
}

void TweenManager::Add(const EaseType & type, Vector3 * from, const Vector3& to, const float d, const std::function<void()>& callback, const float s)
{
	Add(from, type, *from, to - *from, d, callback, s);
}

void TweenManager::Loop(float * value, const EaseType & type, const float b, const float c, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 0);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::Loop(Vector2 * value, const EaseType & type, const Vector2 & b, const Vector2 c, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 0);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::Loop(Vector3 * value, const EaseType & type, const Vector3 & b, const Vector3& c, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 0);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::Loop(const EaseType & type, float * from, const float to, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 0);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::Loop(const EaseType & type, Vector2 * from, const Vector2 to, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 0);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::Loop(const EaseType & type, Vector3 * from, const Vector3 & to, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 0);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::LoopOnce(float * value, const EaseType & type, const float b, const float c, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 1);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::LoopOnce(Vector2 * value, const EaseType & type, const Vector2 & b, const Vector2 & c, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 1);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::LoopOnce(Vector3 * value, const EaseType & type, const Vector3 & b, const Vector3 & c, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 1);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::LoopOnce(const EaseType & type, float * from, const float to, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 1);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::LoopOnce(const EaseType & type, Vector2 * from, const Vector2 to, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 1);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::LoopOnce(const EaseType & type, Vector3 * from, const Vector3 & to, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, 1);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::LoopCount(float * value, const EaseType & type, const int count, const float b, const float c, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, count);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::LoopCount(Vector2 * value, const EaseType & type, const int count, const Vector2 b, const Vector2 c, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, count);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::LoopCount(Vector3 * value, const EaseType & type, const int count, const Vector3 & b, const Vector3 & c, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, count);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::LoopCount(const EaseType & type, const int count, float * from, const float to, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, count);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::LoopCount(const EaseType & type, const int count, Vector2 * from, const Vector2 to, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, count);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::LoopCount(const EaseType & type, const int count, Vector3 * from, const Vector3 & to, const float d, const std::function<void()>& callback, const float s)
{
	auto option = TweenOptionParam(EasingUpdateType::Loop, count);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::LoopPingPong(float * value, const EaseType & type, const float b, const float c, const float d, const std::function<void()>& callback, const float s, int count)
{
	auto option = TweenOptionParam(EasingUpdateType::PingPong, count);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::LoopPingPong(Vector2 * value, const EaseType & type, const Vector2 b, const Vector2 c, const float d, const std::function<void()>& callback, const float s, int count)
{
	auto option = TweenOptionParam(EasingUpdateType::PingPong, count);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::LoopPingPong(Vector3 * value, const EaseType & type, const Vector3 & b, const Vector3 & c, const float d, const std::function<void()>& callback, const float s, int count)
{
	auto option = TweenOptionParam(EasingUpdateType::PingPong, count);
	Add(value, type, b, c, d / 2.0f, callback, s, option);
}

void TweenManager::LoopPingPong(const EaseType & type, float * from, const float to, const float d, const std::function<void()>& callback, const float s, int count)
{
	auto option = TweenOptionParam(EasingUpdateType::PingPong, count);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::LoopPingPong(const EaseType & type, Vector2 * from, const Vector2 to, const float d, const std::function<void()>& callback, const float s, int count)
{
	auto option = TweenOptionParam(EasingUpdateType::PingPong, count);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::LoopPingPong(const EaseType & type, Vector3 * from, const Vector3 & to, const float d, const std::function<void()>& callback, const float s, int count)
{
	auto option = TweenOptionParam(EasingUpdateType::PingPong, count);
	Add(from, type, *from, to - *from, d / 2.0f, callback, s, option);
}

void TweenManager::Delay(const float delay, const std::function<void()>& func, float * debug)
{
	Add(debug, Linear, 0.0f, 1.0f, delay, func);
}

bool TweenManager::Cancel(const void * address)
{
	for (auto& tween : tweenList_)
	{
		if (tween->GetAddress() != address)
			continue;

		tween->End();
		return true;
	}
	return false;
}

bool TweenManager::DelayCancel(const void * address)
{
	return Cancel(address);
}
