#include "Model.h"
#include "../DirectX/DirectX11.h"
#include "../ConstantBuffer/ConstantBuffer.h"
#include "../util/math/Matrix.h"

Model::Model() :
	m_AlphaBlend(false, false)
{
	DirectX11::GetInstance()->CreateBuffer(&m_pConstantBuffer, nullptr, sizeof(WorldCB), D3D11_BIND_CONSTANT_BUFFER);
}

Model::~Model()
{
}

void Model::Draw(Mesh::Shader& shader, const Matrix& world)
{
	// 定数バッファ情報の送信
	WorldCB cb{
		Matrix::Transpose(world)
	};

	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);
	DirectX11::GetInstance()->GetContext()->VSSetConstantBuffers(1, 1, &m_pConstantBuffer.p);

	// アルファブレンドの設定
	DirectX11::GetInstance()->SetAlphaBlend(m_AlphaBlend.first, m_AlphaBlend.second);
	
	// 描画
	mesh.Draw(shader);

	// アルファブレンドの設定
	DirectX11::GetInstance()->SetAlphaBlend(false, false);
}

void Model::DrawForShadow(Mesh::Shader& shader, const Matrix& world)
{
	// 定数バッファ情報の送信
	WorldCB cb {
		Matrix::Transpose(world)
	};

	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);
	DirectX11::GetInstance()->GetContext()->VSSetConstantBuffers(1, 1, &m_pConstantBuffer.p);

	// 描画
	mesh.Draw(shader);
}

void Model::SetAlphaBlend(bool alphaEnable, bool alphaToCoverageEnable)
{
	m_AlphaBlend = std::pair<bool, bool>(alphaEnable, alphaToCoverageEnable);
}
