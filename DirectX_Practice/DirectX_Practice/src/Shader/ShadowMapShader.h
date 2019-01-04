#pragma once
#include "Effect.h"
#include "../Graphic/Model/Mesh.h"

class ShadowMapShader : public Mesh::Shader
{
public:
	// コンストラクタ
	explicit ShadowMapShader(Effect & effect);
	// シェーダー開始
	virtual void Begin() override;
	// シェーダー終了
	virtual void End() override;
	// マテリアルの設定
	virtual void Material(const Mesh::Material& material, ID3D11Buffer* constantBuffer, ID3D11SamplerState* sampler) override;

protected:
	Effect			m_Effect;						// シェーダー
};