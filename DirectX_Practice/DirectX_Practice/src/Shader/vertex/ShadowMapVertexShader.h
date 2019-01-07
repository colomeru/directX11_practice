#pragma once
#include "VertexShader.h"

class ShadowMapVertexShader : public VertexShader
{
public:
	// コンストラクタ
	ShadowMapVertexShader();

protected:
	// インプットレイアウトの生成
	virtual HRESULT CreateInputLayout() override;
};
