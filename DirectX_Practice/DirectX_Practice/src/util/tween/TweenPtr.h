#pragma once
#include <memory>

class TweenObject;
// tween用シェアドポインタ
using TweenPtr = std::shared_ptr<TweenObject>;