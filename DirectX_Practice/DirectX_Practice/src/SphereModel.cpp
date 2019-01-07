#include "SphereModel.h"
#include "Vertex.h"
#include "DirectX/DirectX11.h"
#include "ConstantBuffer/ConstantBuffer.h"
#include "Camera/Camera.h"
#include "util/math/MathHelper.h"

Vertex*		m_pVertices;
int*		m_pIndexes;

SphereModel::SphereModel(float x) :
	m_VertexNum((SEGMENT + 1) * (SEGMENT / 2 + 1)),
	m_IndexNum(SEGMENT * 3 + SEGMENT * (SEGMENT / 2 - 1) * 6 + SEGMENT * 3),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_pConstantBuffer(nullptr),
	m_stride(sizeof(Vertex)),
	x(0.0f),
	posX(x)
{
	DirectX11::GetInstance()->CreateBuffer(&m_pVertexBuffer, (void*)CreateVertex(), sizeof(Vertex) * GetVertexNum(), D3D11_BIND_VERTEX_BUFFER);
	DirectX11::GetInstance()->CreateBuffer(&m_pIndexBuffer, (void*)CreateIndex(), sizeof(int) * GetIndexNum(), D3D11_BIND_INDEX_BUFFER);
	DirectX11::GetInstance()->CreateBuffer(&m_pConstantBuffer, nullptr, sizeof(WorldProjCB), D3D11_BIND_CONSTANT_BUFFER);
}

SphereModel::~SphereModel()
{
	delete[] m_pVertices;
	delete[] m_pIndexBuffer;
}

void SphereModel::Draw()
{
	//XMMATRIX worldMatrix = XMMatrixRotationY(x += 0.001f);
	//worldMatrix *= XMMatrixTranslation(posX, 0.0f, 0.0f);

	WorldCB cd;
	cd.world		= Matrix::Identity;

	unsigned int offset = 0;

	DirectX11::GetInstance()->GetContext()->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cd, 0, 0);
	DirectX11::GetInstance()->GetContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	DirectX11::GetInstance()->GetContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_stride, &offset);
	DirectX11::GetInstance()->GetContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	DirectX11::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// •`‰æ
	DirectX11::GetInstance()->GetContext()->DrawIndexed(GetIndexNum(), 0, 0);
}

Vertex * SphereModel::CreateVertex()
{
	m_pVertices = new Vertex[m_VertexNum];

	for (int i = 0; i <= (SEGMENT / 2); ++i)
	{
		float irad  = MathHelper::Pi * 2.0f / (float)SEGMENT * (float)i;
		//float y		= (float)cos(irad);
		//float r		= (float)sin(irad);
		float y		= MathHelper::Cos(MathHelper::ToDegrees(irad));
		float r		= MathHelper::Sin(MathHelper::ToDegrees(irad));
		float v		= (float)i / (float)(SEGMENT / 2);
		for (int j = 0; j <= SEGMENT; ++j)
		{
			float jrad = MathHelper::Pi * 2.0f / (float)SEGMENT * (float)j;
			//float x = r * (float)cos(jrad);
			//float z = r * (float)sin(jrad);
			float x = r * MathHelper::Cos(MathHelper::ToDegrees(jrad));
			float z = r * MathHelper::Sin(MathHelper::ToDegrees(jrad));
			float u = (float)j / (float)SEGMENT;
			int inx = i * (SEGMENT + 1) + j;
			m_pVertices[inx].Position.x = x;
			m_pVertices[inx].Position.y = y;
			m_pVertices[inx].Position.z = z;
			m_pVertices[inx].Normal.x	= x;
			m_pVertices[inx].Normal.y	= y;
			m_pVertices[inx].Normal.z	= z;
			m_pVertices[inx].Texcoord.x	= u;
			m_pVertices[inx].Texcoord.y	= v;
		}
	}
	return m_pVertices;
}

int * SphereModel::CreateIndex()
{
	int icount = m_IndexNum;
	m_pIndexes = new int[icount];
	
	m_IndexNum = 0;
	int i = 0;

	for (int j = 0; j < SEGMENT; ++j)
	{
		m_pIndexes[m_IndexNum	 ] =  i		 * (SEGMENT + 1) + j;
		m_pIndexes[m_IndexNum + 1] = (i + 1) * (SEGMENT + 1) + j + 1;
		m_pIndexes[m_IndexNum + 2] = (i + 1) * (SEGMENT + 1) + j;
		m_IndexNum += 3;
	}
	for (i = 1; i < SEGMENT / 2; ++i)
	{
		for (int j = 0; j < SEGMENT; j++)
		{
			m_pIndexes[m_IndexNum	 ] =  i		 * (SEGMENT + 1) + j;
			m_pIndexes[m_IndexNum + 1] =  i		 * (SEGMENT + 1) + j + 1;
			m_pIndexes[m_IndexNum + 2] = (i + 1) * (SEGMENT + 1) + j;
			m_IndexNum += 3;
			m_pIndexes[m_IndexNum	 ] =  i		 * (SEGMENT + 1) + j + 1;
			m_pIndexes[m_IndexNum + 1] = (i + 1) * (SEGMENT + 1) + j + 1;
			m_pIndexes[m_IndexNum + 2] = (i + 1) * (SEGMENT + 1) + j;
			m_IndexNum += 3;
		}
	}
	i = SEGMENT / 2;
	for (int j = 0; j < SEGMENT; ++j)
	{
		m_pIndexes[m_IndexNum	 ] =  i		 * (SEGMENT + 1) + j;
		m_pIndexes[m_IndexNum + 1] = (i + 1) * (SEGMENT + 1) + j + 1;
		m_pIndexes[m_IndexNum + 2] = (i + 1) * (SEGMENT + 1) + j;
		m_IndexNum += 3;
	}

	return m_pIndexes;
}

int SphereModel::GetVertexNum() const
{
	return m_VertexNum;
}

int SphereModel::GetIndexNum() const
{
	return m_IndexNum;
}
