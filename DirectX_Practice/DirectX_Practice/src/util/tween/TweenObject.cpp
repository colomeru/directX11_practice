#include "TweenObject.h"
#include "../math/MathHelper.h"

TweenObject::TweenObject(void * address, const TweenFuncParam& func, const EasingUpdateType& type, float d, float s, const std::function<void()>& action, int loopCount) :
	address_(address),
	func_(func),
	updateFunc_(GetFunctionType(type)),
	d_(d), s_(s),
	action_(action),
	timer_(0.0f),
	timeScale_(1.0f),
	isEnd_(false),
	loopCount_(loopCount),
	sine_(1.0f)
{
	values_.clear();
}

TweenObject::~TweenObject()
{
	values_.clear();
	action_ = nullptr;
}

void TweenObject::Update(const float deltaTime)
{
	if (!isEnd_)
		updateFunc_(deltaTime);
}

bool TweenObject::IsEnd() const
{
	return isEnd_;
}

void TweenObject::End()
{
	isEnd_ = true;
	action_ = nullptr;
}

void TweenObject::Play(float scale)
{
	timeScale_ = scale;
}

void TweenObject::Stop()
{
	timeScale_ = 0.0f;
}

void TweenObject::AddValue(float * v, float b, float c)
{
	values_.push_back(EaseParam(v, b, c));
}

void * TweenObject::GetAddress() const
{
	return address_;
}

std::function<void(float)> TweenObject::GetFunctionType(const EasingUpdateType & type)
{
	switch (type)
	{
	case EasingUpdateType::Common:   return [=](float delta) { CommonUpdate(delta); }; break;
	case EasingUpdateType::Loop:	 return [=](float delta) { LoopUpdate(delta);	};	break;
	case EasingUpdateType::PingPong: return [=](float delta) { PingPongUpdate(delta);	}; break;
	default: return [](float deltaTime) {};
	}
}

void TweenObject::CommonUpdate(float deltaTime)
{
	timer_ += deltaTime * timeScale_;
	timer_ = MathHelper::Min(timer_, d_);

	for (auto e : values_)
	{
		if (e.value != nullptr)
			*e.value = func_(timer_, e.b, e.c, d_, s_);
	}

	if (timer_ >= d_)
	{
		Invoke();
		isEnd_ = true;
		action_ = nullptr;
	}
}

void TweenObject::LoopUpdate(float deltaTime)
{
	timer_ += deltaTime * timeScale_;
	timer_ = MathHelper::Min(timer_, d_);

	for (auto e : values_)
		*e.value = func_(timer_, e.b, e.c, d_, s_);


	if (timer_ >= d_)
	{
		// 初期値と移動量を反転
		for (auto& ease : values_)
		{
			ease.b += ease.c;
			ease.c *= -1.0f;
		}

		timer_ = 0.0f;
		sine_ *= -1.0f;
		 // 行きフラグ | sine_ < 0 < sine_  | 帰りフラグ
		if (sine_ > 0.0f)
		{
			Invoke();
			loopCount_--;
			loopCount_ = MathHelper::Max(loopCount_, -1);

			// ループ終了
			if (loopCount_ == 0)
				isEnd_ = true;
		}
	}
}

void TweenObject::PingPongUpdate(float deltaTime)
{
	timer_ += deltaTime * timeScale_ * sine_;
	timer_ = MathHelper::Clamp(timer_, 0.0f, d_);

	for (auto e : values_)
		*e.value = func_(timer_, e.b, e.c, d_, s_);


	if (timer_ >= d_)
	{
		sine_ *= -1.0f;
	}
	else if (timer_ <= 0.0f)
	{
		sine_ *= -1.0f;

		Invoke();
		loopCount_--;
		loopCount_ = MathHelper::Max(loopCount_, -1);

		// ループ終了
		if (loopCount_ == 0)
			isEnd_ = true;
	}
}

void TweenObject::Invoke()
{
	if (action_ != nullptr)
		action_();
}
