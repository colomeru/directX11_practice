#pragma once
#include "VertexShader.h"

class FontVertexShader : public VertexShader
{
public:
	// �R���X�g���N�^
	FontVertexShader();

protected:
	// �C���v�b�g���C�A�E�g�̐���
	virtual HRESULT CreateInputLayout() override;
};
