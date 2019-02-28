#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include "../../util/math/Color.h"

#include <vector>
#include "../Reader/PMXFileReader.h"
#include "../Reader/PMDFileReader.h"
#include <functional>

struct Vertex;

class Mesh
{
public:
	// マテリアル構造体
	struct Material
	{
		Color 			ambient;						// 環境光カラー
		Color 			diffuse;						// 拡散反射光カラー
		Color 			specular;						// 鏡面反射光カラー
		Color 			emission;						// 自己照明カラー
		float			shininess;						// 鏡面反射指数
		CComPtr<ID3D11ShaderResourceView>	pTexture;	// テクスチャ
		D3D11_CULL_MODE	cullMode;						// 描画カリングモード
	};
	// サブセット構造体
	struct Subset
	{
		int						material;		// マテリアル
		int						start;			// インデクスの開始位置
		int						count;			// インデックス数
	};
	// メッシュシェーダークラス
	class Shader 
	{
	public:
		// 仮想デストラクタ
		virtual ~Shader() {}
		// 開始
		virtual void Begin() = 0;
		// 終了
		virtual void End() = 0;
		// マテリアルの設定
		virtual void Material(const Mesh::Material& material, ID3D11Buffer* constantBuffer, ID3D11SamplerState* sampler) = 0;
	};

public:
	// コンストラクタ
	Mesh();
	// デストラクタ
	~Mesh();
	// 描画
	void Draw(Mesh::Shader& shader) const;
	// PMDファイルの読み込み
	void Load(const mmd::PMD::PMDFile & pmd, const std::string & folderPATH);
	// PMXファイルの読み込み
	void Load(const mmd::pmx::PMXFile & pmx, const std::string & folderPATH);
	
	// コピー禁止
	//Mesh(const Mesh&) = delete;
	//Mesh& operator = (const Mesh&) = delete;

private:
	//
	WCHAR* ToWCHAR(WCHAR* wc, const std::string & str);
	// 頂点バッファの生成
	void CreateVertexBuffer(void * resources, int size);
	// インデックスバッファの生成
	void CreateIndexBuffer(void * resources, int size);

	// PMD_頂点リストの読み込み
	void LoadVertices(const mmd::PMD::Vertices& pmdvertices);
	// PMX_頂点リストの読み込み
	void LoadVertices(const mmd::pmx::Vertices& pmxVertices);
	// 頂点データのセット
	void SetVertexParameter(Vertex & vertex, std::function<void(Vertex&)> func);

private:
	// マテリアル配列
	std::vector<Material>	m_Materials;
	// サブセット配列
	std::vector<Subset>		m_Subsets;

	CComPtr<ID3D11Buffer>			m_pVertexBuffer;	// 頂点バッファ
	CComPtr<ID3D11Buffer>			m_pIndexBuffer;		// インデックスバッファ
	CComPtr<ID3D11Buffer>			m_pConstantBuffer;	// コンストラクタバッファ
	CComPtr<ID3D11SamplerState>		m_pSampler;			// サンプラー
};