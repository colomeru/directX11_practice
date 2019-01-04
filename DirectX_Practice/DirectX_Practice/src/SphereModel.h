#pragma once
#include <d3d11.h>

struct Vertex;

class SphereModel
{
public:
	// コンストラクタ
	SphereModel(float x);
	// デストラクタ
	~SphereModel();

	//
	void Draw();

	Vertex* CreateVertex();
	int*	CreateIndex();
	int		GetVertexNum() const;
	int		GetIndexNum() const;

	bool	Clear();

private:
	const int	SEGMENT = 36;

	int			m_VertexNum;				// 頂点数
	int			m_IndexNum;					// インデックス数

	ID3D11Buffer*	m_pVertexBuffer;		// 頂点バッファ
	ID3D11Buffer*	m_pIndexBuffer;			// インデックスバッファ
	ID3D11Buffer*	m_pConstantBuffer;		// 定数バッファ
	unsigned int	m_stride;

	float			x;
	float			posX;
};