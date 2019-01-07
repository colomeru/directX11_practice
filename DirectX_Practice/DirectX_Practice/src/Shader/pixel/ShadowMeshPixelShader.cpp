#include "ShadowMeshPixelShader.h"

ShadowMeshPixelShader::ShadowMeshPixelShader() :
	PixelShader("ShadowPixelShader.hlsl")
{
	CreateShader();
}
