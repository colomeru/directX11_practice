#include "MeshShader.h"
#include "../DirectX/DirectX11.h"
#include "../ConstantBuffer/ConstantBuffer.h"

MeshShader::MeshShader(Effect & effect) :
	m_Effect(effect),
	m_pConstantBuffer(nullptr),
	m_pSampler(nullptr)
{
}

void MeshShader::Begin()
{
	// シェーダーをセット
	m_Effect.Begin();

	// マテリアル情報をシェーダーに送信
	MaterialCB cb{
		m_Material.ambient,
		m_Material.diffuse,
		m_Material.specular
	};
	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer.p, 0, NULL, &cb, 0, 0);
	DirectX11::GetInstance()->GetContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer.p);
	// テクスチャ情報をシェーダーに送信
	DirectX11::GetInstance()->GetContext()->PSSetShaderResources(0, 1, &m_Material.pTexture.p);
	DirectX11::GetInstance()->GetContext()->PSSetSamplers(0, 1, &m_pSampler.p);
}

void MeshShader::End()
{
	m_Effect.End();

	CComPtr<ID3D11ShaderResourceView> pNullSRV;
	DirectX11::GetInstance()->GetContext()->PSSetShaderResources(0, 1, &pNullSRV.p);
	CComPtr<ID3D11SamplerState>	pNullSamper;
	DirectX11::GetInstance()->GetContext()->PSSetSamplers(0, 1, &m_pSampler.p);
}

void MeshShader::Material(const Mesh::Material& material, ID3D11Buffer* constantBuffer, ID3D11SamplerState* sampler)
{
	m_Material		  = material;
	m_pConstantBuffer = constantBuffer;
	m_pSampler		  = sampler;
}
