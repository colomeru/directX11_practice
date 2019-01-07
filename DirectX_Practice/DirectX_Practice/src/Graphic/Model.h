#pragma once
#include "Model/Mesh.h"
#include "Model/Skeleton.h"

struct Matrix;

class Model
{
public:
	// �R���X�g���N�^
	Model();
	// �f�X�g���N�^
	~Model();
	// �`��
	void Draw(Mesh::Shader& shader, const Matrix& world);
	// �e�p�`��
	void DrawForShadow(Mesh::Shader& shader, const Matrix& world);

	// �A���t�@�u�����h�̐ݒ�
	void SetAlphaBlend(bool alphaEnable, bool alphaToCoverageEnable);

	// �R�s�[�֎~
	Model(const Model&) = delete;
	Model& operator = (const Model&) = delete;

public:
	Mesh		mesh;		// ���b�V��
	Skeleton	skeleton;	// �X�P���g��

private:
	/// <bool alphaEnable, bool alphaToCoverageEnable>
	typedef std::pair<bool, bool>	AlphaBlend;
	AlphaBlend				m_AlphaBlend;	// �A���t�@�u�����h
	CComPtr<ID3D11Buffer>	m_pConstantBuffer;	// �萔�o�b�t�@
};
