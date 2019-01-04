#pragma once
#include "VertexShader.h"

class FontVertexShader : public VertexShader
{
public:
	// コンストラクタ
	FontVertexShader();

protected:
	// インプットレイアウトの生成
	virtual HRESULT CreateInputLayout() override;
};
