#pragma once
#include "VertexShader.h"

class SkinnedMeshVertexShader : public VertexShader
{
public:
	// コンストラクタ
	SkinnedMeshVertexShader();

protected:
	// インプットレイアウトの生成
	virtual HRESULT CreateInputLayout() override;
};
