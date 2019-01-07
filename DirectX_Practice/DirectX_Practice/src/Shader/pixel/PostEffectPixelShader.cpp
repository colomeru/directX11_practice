#include "PostEffectPixelShader.h"

PostEffectPixelShader::PostEffectPixelShader(const std::string & fileName, const std::string& entryPoint) :
	PixelShader(fileName, entryPoint)
{
	CreateShader();
}
