#pragma once
#include "VertexShader.h"

class MeshVertexShader : public VertexShader
{
public:
	// コンストラクタ
	MeshVertexShader();

protected:
	// インプットレイアウトの生成
	virtual HRESULT CreateInputLayout() override;
};
