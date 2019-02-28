#include "MeshShader.h"
#include "../DirectX/DirectX11.h"
#include "../ConstantBuffer/ConstantBuffer.h"
#include "../DirectX/ShaderResource.h"

MeshShader::MeshShader(Effect & effect) :
	m_Effect(effect),
	m_Material(),
	m_pConstantBuffer(nullptr),
	m_pSampler(nullptr)
{
}

void MeshShader::Begin()
{
	// �V�F�[�_�[���Z�b�g
	m_Effect.Begin();

	DirectX11::GetInstance()->SetRasterizer(D3D11_FILL_MODE::D3D11_FILL_SOLID, m_Material.cullMode);

	// �}�e���A�������V�F�[�_�[�ɑ��M
	MaterialCB cb{
		m_Material.ambient,
		m_Material.diffuse,
		m_Material.specular
	};
	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer.p, 0, NULL, &cb, 0, 0);
	DirectX11::GetInstance()->GetContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer.p);
	// �e�N�X�`�������V�F�[�_�[�ɑ��M
	DirectX11::GetInstance()->GetContext()->PSSetShaderResources(0, 1, &m_Material.pTexture.p);
	DirectX11::GetInstance()->GetContext()->PSSetSamplers(0, 1, &m_pSampler.p);
}

void MeshShader::End()
{
	m_Effect.End();

	ShaderResource clearRes;
	clearRes.Set();
}

void MeshShader::Material(const Mesh::Material& material, ID3D11Buffer* constantBuffer, ID3D11SamplerState* sampler)
{
	m_Material		  = material;
	m_pConstantBuffer = constantBuffer;
	m_pSampler		  = sampler;
}
