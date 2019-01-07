#pragma once
#include "Effect.h"
#include "../Graphic/Model/Mesh.h"

class MeshShader : public Mesh::Shader
{
public:
	// コンストラクタ
	explicit MeshShader(Effect & effect);
	// シェーダー開始
	virtual void Begin() override;
	// シェーダー終了
	virtual void End() override;
	// マテリアルの設定
	virtual void Material(const Mesh::Material& material, ID3D11Buffer* constantBuffer, ID3D11SamplerState* sampler) override;

protected:
	Effect			m_Effect;						// シェーダー
	Mesh::Material	m_Material;						// マテリアル
	CComPtr<ID3D11Buffer>		m_pConstantBuffer;	// 定数バッファ
	CComPtr<ID3D11SamplerState>	m_pSampler;			// サンプラー
};