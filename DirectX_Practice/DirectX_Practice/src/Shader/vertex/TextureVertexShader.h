#pragma once
#include "VertexShader.h"

class TextureVertexShader : public VertexShader
{
public:
	// コンストラクタ
	TextureVertexShader();

protected:
	// インプットレイアウトの生成
	virtual HRESULT CreateInputLayout() override;
};
