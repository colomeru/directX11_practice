#pragma once
#include <d3d11.h>
#include <atlbase.h>

class SphereModel
{
public:
	// �R���X�g���N�^
	SphereModel();
	// �f�X�g���N�^
	~SphereModel();
	//
	void Draw();

private:
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	int	GetVertexNum() const;
	int	GetIndexNum() const;

private:
	const int	SEGMENT = 36;

	int						m_VertexNum;		// ���_��
	int						m_IndexNum;			// �C���f�b�N�X��
	CComPtr<ID3D11Buffer>	m_pVertexBuffer;	// ���_�o�b�t�@
	CComPtr<ID3D11Buffer>	m_pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	CComPtr<ID3D11Buffer>	m_pConstantBuffer;	// �萔�o�b�t�@
};