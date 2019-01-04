#pragma once
#include <d3d11.h>

struct Vertex;

class SphereModel
{
public:
	// �R���X�g���N�^
	SphereModel(float x);
	// �f�X�g���N�^
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

	int			m_VertexNum;				// ���_��
	int			m_IndexNum;					// �C���f�b�N�X��

	ID3D11Buffer*	m_pVertexBuffer;		// ���_�o�b�t�@
	ID3D11Buffer*	m_pIndexBuffer;			// �C���f�b�N�X�o�b�t�@
	ID3D11Buffer*	m_pConstantBuffer;		// �萔�o�b�t�@
	unsigned int	m_stride;

	float			x;
	float			posX;
};