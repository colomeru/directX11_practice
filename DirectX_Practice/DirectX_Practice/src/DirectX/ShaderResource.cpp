#include "ShaderResource.h"
#include "DirectX11.h"

ShaderResource::ShaderResource()
{
}

void ShaderResource::Set(int slot) const
{
	DirectX11::GetInstance()->GetContext()->PSSetShaderResources(slot, 1, &pSRV.p);
	DirectX11::GetInstance()->GetContext()->PSSetSamplers(slot, 1, &pSampler.p);
}
