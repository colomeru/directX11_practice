#pragma once
#include "PixelShader.h"
#include <string>

class PostEffectPixelShader : public PixelShader
{
public:
	// コンストラクタ
	PostEffectPixelShader(const std::string& fileName, const std::string& entryPoint = "main");
};