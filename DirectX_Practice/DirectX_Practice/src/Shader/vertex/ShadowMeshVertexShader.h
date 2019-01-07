#pragma once
#include "VertexShader.h"

class ShadowMeshVertexShader : public VertexShader
{
public:
	// コンストラクタ
	ShadowMeshVertexShader();

protected:
	// インプットレイアウトの生成
	virtual HRESULT CreateInputLayout() override;
};
