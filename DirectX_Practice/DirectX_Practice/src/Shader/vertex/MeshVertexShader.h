#pragma once
#include "VertexShader.h"

class MeshVertexShader : public VertexShader
{
public:
	// �R���X�g���N�^
	MeshVertexShader();

protected:
	// �C���v�b�g���C�A�E�g�̐���
	virtual HRESULT CreateInputLayout() override;
};
