#include "SphereModel.h"
#include "Vertex.h"
#include "DirectX/DirectX11.h"
#include "ConstantBuffer/ConstantBuffer.h"
#include "util/math/MathHelper.h"
#include <vector>
#include "Camera/Camera.h"

SphereModel::SphereModel() :
	m_VertexNum((SEGMENT + 1) * (SEGMENT / 2 + 1)),
	m_IndexNum(SEGMENT * 3 + SEGMENT * (SEGMENT / 2 - 1) * 6 + SEGMENT * 3),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_pConstantBuffer(nullptr)
{
	DirectX11::GetInstance()->CreateBuffer(&m_pConstantBuffer, nullptr, sizeof(WorldProjCB), D3D11_BIND_CONSTANT_BUFFER);
	CreateVertexBuffer();
	CreateIndexBuffer();
}

SphereModel::~SphereModel()
{
}

void SphereModel::Draw()
{
	WorldProjCB cd;
	auto world = Matrix::CreateScale(Vector3(1000.0f, 1000.0f, 1000.0f))
			   * Matrix::CreateTranslation(Camera::GetInstance()->GetPosition());
	auto proj  = Matrix::CreatePerspectiveFieldOfView(60.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	cd.world = Matrix::Transpose(world);
	cd.projection = Matrix::Transpose(proj);

	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cd, 0, 0);
	DirectX11::GetInstance()->GetContext()->VSSetConstantBuffers(1, 1, &m_pConstantBuffer.p);

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	DirectX11::GetInstance()->GetContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer.p, &stride, &offset);
	DirectX11::GetInstance()->GetContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	DirectX11::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// •`‰æ
	DirectX11::GetInstance()->GetContext()->DrawIndexed(GetIndexNum(), 0, 0);
}

void SphereModel::CreateVertexBuffer()
{
	auto pVertices = std::vector<Vertex>(m_VertexNum);

	for (int i = 0; i <= (SEGMENT / 2); ++i)
	{
		float irad  = MathHelper::Pi * 2.0f / (float)SEGMENT * (float)i;
		float y		= MathHelper::Cos(MathHelper::ToDegrees(irad));
		float r		= MathHelper::Sin(MathHelper::ToDegrees(irad));
		float v		= (float)i / (float)(SEGMENT / 2);
		for (int j = 0; j <= SEGMENT; ++j)
		{
			float jrad = MathHelper::Pi * 2.0f / (float)SEGMENT * (float)j;
			float x = r * MathHelper::Cos(MathHelper::ToDegrees(jrad));
			float z = r * MathHelper::Sin(MathHelper::ToDegrees(jrad));
			float u = (float)j / (float)SEGMENT;
			int inx = i * (SEGMENT + 1) + j;
			pVertices[inx].Position.x = x;
			pVertices[inx].Position.y = y;
			pVertices[inx].Position.z = z;
			pVertices[inx].Normal.x	= x;
			pVertices[inx].Normal.y	= y;
			pVertices[inx].Normal.z	= z;
			pVertices[inx].Texcoord.x = u;
			pVertices[inx].Texcoord.y = v;
		}
	}
	DirectX11::GetInstance()->CreateBuffer(&m_pVertexBuffer, (void*)pVertices.data(), sizeof(Vertex) * GetVertexNum(), D3D11_BIND_VERTEX_BUFFER);
}

void SphereModel::CreateIndexBuffer()
{
	std::vector<int> pIndices(m_IndexNum);

	m_IndexNum = 0;
	int i = 0;

	for (int j = 0; j < SEGMENT; ++j)
	{
		pIndices[m_IndexNum	   ] =  i	   * (SEGMENT + 1) + j;
		pIndices[m_IndexNum + 1] = (i + 1) * (SEGMENT + 1) + j + 1;
		pIndices[m_IndexNum + 2] = (i + 1) * (SEGMENT + 1) + j;
		m_IndexNum += 3;
	}
	for (i = 1; i < SEGMENT / 2; ++i)
	{
		for (int j = 0; j < SEGMENT; j++)
		{
			pIndices[m_IndexNum	   ] =  i	   * (SEGMENT + 1) + j;
			pIndices[m_IndexNum + 1] =  i	   * (SEGMENT + 1) + j + 1;
			pIndices[m_IndexNum + 2] = (i + 1) * (SEGMENT + 1) + j;
			m_IndexNum += 3;
			pIndices[m_IndexNum	   ] =  i	   * (SEGMENT + 1) + j + 1;
			pIndices[m_IndexNum + 1] = (i + 1) * (SEGMENT + 1) + j + 1;
			pIndices[m_IndexNum + 2] = (i + 1) * (SEGMENT + 1) + j;
			m_IndexNum += 3;
		}
	}
	i = SEGMENT / 2;
	for (int j = 0; j < SEGMENT; ++j)
	{
		pIndices[m_IndexNum	   ] =  i	   * (SEGMENT + 1) + j;
		pIndices[m_IndexNum + 1] = (i + 1) * (SEGMENT + 1) + j + 1;
		pIndices[m_IndexNum + 2] = (i + 1) * (SEGMENT + 1) + j;
		m_IndexNum += 3;
	}
	DirectX11::GetInstance()->CreateBuffer(&m_pIndexBuffer, (void*)pIndices.data(), sizeof(int) * GetIndexNum(), D3D11_BIND_INDEX_BUFFER);
}

int SphereModel::GetVertexNum() const
{
	return m_VertexNum;
}

int SphereModel::GetIndexNum() const
{
	return m_IndexNum;
}
