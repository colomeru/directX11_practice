#pragma once
#include "VertexShader.h"

class SkinnedMeshVertexShader : public VertexShader
{
public:
	// �R���X�g���N�^
	SkinnedMeshVertexShader();

protected:
	// �C���v�b�g���C�A�E�g�̐���
	virtual HRESULT CreateInputLayout() override;
};
