#pragma once
#include "VertexShader.h"

class ShadowMeshVertexShader : public VertexShader
{
public:
	// �R���X�g���N�^
	ShadowMeshVertexShader();

protected:
	// �C���v�b�g���C�A�E�g�̐���
	virtual HRESULT CreateInputLayout() override;
};
