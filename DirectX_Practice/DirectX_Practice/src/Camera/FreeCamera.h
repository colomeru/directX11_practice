#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include "../util/math/Matrix.h"

struct Vector3;

class FreeCamera
{
public:
	// �R���X�g���N�^
	FreeCamera();
	// �f�X�g���N�^
	~FreeCamera();
	// �X�V
	void Update();
	// �`��
	void Draw() const;

	// ���s�ړ�
	void Translation(const Vector3& v);

private:
	CComPtr<ID3D11Buffer>	m_pConstantBuffer;	// �R���X�^���g�o�b�t�@
	Matrix					m_pose;				// �p��
};
