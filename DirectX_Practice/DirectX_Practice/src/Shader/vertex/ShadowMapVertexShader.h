#pragma once
#include "VertexShader.h"

class ShadowMapVertexShader : public VertexShader
{
public:
	// �R���X�g���N�^
	ShadowMapVertexShader();

protected:
	// �C���v�b�g���C�A�E�g�̐���
	virtual HRESULT CreateInputLayout() override;
};
