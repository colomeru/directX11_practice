#pragma once
#include "VertexShader.h"

class TextureVertexShader : public VertexShader
{
public:
	// �R���X�g���N�^
	TextureVertexShader();

protected:
	// �C���v�b�g���C�A�E�g�̐���
	virtual HRESULT CreateInputLayout() override;
};
