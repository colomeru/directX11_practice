#pragma once
#include "PixelShader.h"
#include <string>

class PostEffectPixelShader : public PixelShader
{
public:
	// �R���X�g���N�^
	PostEffectPixelShader(const std::string& fileName, const std::string& entryPoint = "main");
};