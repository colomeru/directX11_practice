#include "Mesh.h"
#include "../../DirectX/DirectX11.h"
#include "../../Vertex.h"
#include "../../ConstantBuffer/ConstantBuffer.h"

#include <unordered_map>
#include "../../WICTextureLoader/WICTextureLoader.h"
#include "../RenderTexture.h"

Mesh::Mesh() :
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_pConstantBuffer(nullptr),
	m_pSampler(nullptr)
{
	// �萔�o�b�t�@�̐���
	DirectX11::GetInstance()->CreateBuffer(&m_pConstantBuffer, nullptr, sizeof(MaterialCB), D3D11_BIND_CONSTANT_BUFFER);
	
	// �T���v���X�e�[�g�̐ݒ�
	D3D11_SAMPLER_DESC			smpDesc;
	ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
	smpDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	// �T���v���X�e�[�g�̐���
	DirectX11::GetInstance()->GetDevice()->CreateSamplerState(&smpDesc, &m_pSampler);
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Mesh::Shader& shader) const
{
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	// �o�b�t�@�̃Z�b�g
	DirectX11::GetInstance()->GetContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer.p, &stride, &offset);
	DirectX11::GetInstance()->GetContext()->IASetIndexBuffer(m_pIndexBuffer.p, DXGI_FORMAT_R32_UINT, 0);

	DirectX11::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (const auto& subset : m_Subsets)
	{
		shader.Material(m_Materials[subset.material], m_pConstantBuffer, m_pSampler);
		shader.Begin();
		DirectX11::GetInstance()->GetContext()->DrawIndexed(subset.count, subset.start, 0);
		shader.End();
	}
}

void Mesh::Load(const mmd::PMD::PMDFile & pmd, const std::string & folderPATH)
{
	// �e�N�X�`���̃C���f�b�N�X�ꎞ�ۑ��p
	std::unordered_map<std::string, int> textures;
	std::unordered_map<int, CComPtr<ID3D11ShaderResourceView>> ptextures;
	// �e�N�X�`���̓ǂݍ���
	for (int i = 0; i < pmd.mesh.materials.size(); i++)
	{
		auto texNameStr = std::string(pmd.mesh.materials[i].texture_filename);
		// �ǂݍ���łȂ��e�N�X�`����ǂݍ��ݕۑ�����
		if (textures.find(texNameStr) == textures.end())
		{
			CComPtr<ID3D11Resource>				pRes;
			CComPtr<ID3D11ShaderResourceView>	pTextureView;

			// �L���X�g
			WCHAR	wc[100];
			ToWCHAR(wc, folderPATH + texNameStr);

			HRESULT hr = DirectX::CreateWICTextureFromFile(DirectX11::GetInstance()->GetDevice(), wc, &pRes, &pTextureView);
			if (FAILED(hr))
				MessageBox(NULL, _T("Texture_Load_Error"), _T(texNameStr.c_str()), MB_OK);

			// �C���f�b�N�X�̓o�^
			textures[texNameStr] = textures.size();
			ptextures[ptextures.size()] = pTextureView;
		}
	}

	// �}�e���A���p�ɔ��̉摜������
	RenderTexture tex(nullptr);
	tex.Create(32, 32);
	tex.Clear();

	// ���_�f�[�^�̓ǂݍ���
	std::vector<Vertex> vertices(pmd.mesh.vertices.size());
	for (int i = 0; i < pmd.mesh.vertices.size(); i++)
	{
		auto position = pmd.mesh.vertices[i].pos;
		auto normal   = pmd.mesh.vertices[i].normal;
		auto texcoord = pmd.mesh.vertices[i].uv;

		vertices[i].Position = Vector3(position.x, position.y, position.z);
		vertices[i].Normal   = Vector3(normal.x, normal.y, normal.z);
		vertices[i].Texcoord = Vector2(texcoord.x, texcoord.y);
	}

	// �T�u�Z�b�g�̓ǂݍ���
	int start = 0;
	m_Subsets.resize(pmd.mesh.materials.size());
	m_Materials.resize(pmd.mesh.materials.size());
	for (int i = 0; i < m_Subsets.size(); i++)
	{
		// �e�N�X�`��������C���f�b�N�X�����
		auto textureName = std::string(pmd.mesh.materials[i].texture_filename);

		// �e�N�X�`���C���f�b�N�X�̕ۑ�
		//subsets[i].material = textures.count(textureName) > 0 ? textures[textureName] : -1;
		m_Subsets[i].material = i;
		m_Subsets[i].start = start;
		m_Subsets[i].count = pmd.mesh.materials[i].vertex_count;

		start += m_Subsets[i].count;

		auto ambient  = pmd.mesh.materials[i].ambient;
		auto diffuse  = pmd.mesh.materials[i].diffuse;
		auto specular = pmd.mesh.materials[i].specular;

		m_Materials[i].ambient  = Color(ambient.x, ambient.y, ambient.z, 0.0f);
		m_Materials[i].diffuse  = Color(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		m_Materials[i].specular = Color(specular.x, specular.y, specular.z, pmd.mesh.materials[i].specularity);

		if (textures.count(textureName) > 0)
			m_Materials[i].pTexture = ptextures.at(textures.at(textureName));
		else
			m_Materials[i].pTexture = tex.pSRV;
	}

	// �C���f�b�N�X�f�[�^��int�^�ɕϊ�
	std::vector<int> indices(pmd.mesh.indices.size());
	for (int i = 0; i < indices.size(); i++)
		indices[i] = pmd.mesh.indices[i];

	// ���_�o�b�t�@�̐���
	DirectX11::GetInstance()->CreateBuffer(&m_pVertexBuffer, (void*)vertices.data(), sizeof(Vertex) * vertices.size(), D3D11_BIND_VERTEX_BUFFER);
	// �C���f�b�N�X�o�b�t�@�̐���
	DirectX11::GetInstance()->CreateBuffer(&m_pIndexBuffer, (void*)indices.data(), sizeof(int) * indices.size(), D3D11_BIND_INDEX_BUFFER);
}

void Mesh::Load(const mmd::pmx::PMXFile & pmx, const std::string & folderPATH)
{
	// �e�N�X�`���̓ǂݍ���
	std::vector<CComPtr<ID3D11ShaderResourceView>> textures(pmx.mesh.textures.size());
	std::unordered_map<int, CComPtr<ID3D11ShaderResourceView>> ptextures;
	for (int i = 0; i < pmx.mesh.textures.size(); i++)
	{
		CComPtr<ID3D11Resource>				pTexture;
		CComPtr<ID3D11ShaderResourceView>	pTextureView;

		// �t�@�C�������擾
		std::string texNameStr = std::string(pmx.mesh.textures[i].begin(), pmx.mesh.textures[i].end());
		std::string str = "";
		for (int i = 0; i < texNameStr.size(); i++)
			str += &texNameStr[i];

		// �L���X�g
		WCHAR	wc[100];
		ToWCHAR(wc, folderPATH + str);

		HRESULT hr = DirectX::CreateWICTextureFromFile(DirectX11::GetInstance()->GetDevice(), wc, &pTexture, &pTextureView);
		if (FAILED(hr))
			MessageBox(NULL, _T("Texture_Load_Error"), _T(str.c_str()), MB_OK);

		textures[i] = pTextureView;
		ptextures[ptextures.size()] = pTextureView;
	}

	// �}�e���A���p�ɔ��̉摜������
	RenderTexture tex(nullptr);
	tex.Create(32, 32);
	tex.Clear();

	// ���_�f�[�^�̓ǂݍ���
	std::vector<Vertex> vertices(pmx.mesh.vertices.size());
	for (int i = 0; i < pmx.mesh.vertices.size(); i++)
	{
		auto position = pmx.mesh.vertices[i].point;
		auto normal	  = pmx.mesh.vertices[i].normal;
		auto texcoord = pmx.mesh.vertices[i].uv;

		vertices[i].Position = Vector3(position.x, position.y, position.z);
		vertices[i].Normal   = Vector3(normal.x, normal.y, normal.z);
		vertices[i].Texcoord = Vector2(texcoord.x, texcoord.y);
	}

	// �T�u�Z�b�g�̓ǂݍ���
	int start = 0;
	m_Subsets.resize(pmx.mesh.materials.size());
	m_Materials.resize(pmx.mesh.materials.size());
	for (int i = 0; i < m_Subsets.size(); i++)
	{
		//subsets[i].material = pmx.mesh.materials[i].texture_index;
		m_Subsets[i].material = i;
		m_Subsets[i].start	  = start;
		m_Subsets[i].count	  = pmx.mesh.materials[i].vertex_count;

		start += m_Subsets[i].count;

		auto ambient  = pmx.mesh.materials[i].ambient;
		auto diffuse  = pmx.mesh.materials[i].diffuse;
		auto specular = pmx.mesh.materials[i].specular;

		m_Materials[i].ambient  = Color(ambient.x, ambient.y, ambient.z, 0.0f);
		m_Materials[i].diffuse  = Color(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		m_Materials[i].specular = Color(specular.x, specular.y, specular.z, pmx.mesh.materials[i].specularity);

		if (pmx.mesh.materials[i].texture_index >= 0)
			m_Materials[i].pTexture = ptextures[pmx.mesh.materials[i].texture_index];
		else
			m_Materials[i].pTexture = tex.pSRV;
	}

	// ���_�o�b�t�@�̐���
	DirectX11::GetInstance()->CreateBuffer(&m_pVertexBuffer, (void*)vertices.data(), sizeof(Vertex) * vertices.size(), D3D11_BIND_VERTEX_BUFFER);
	// �C���f�b�N�X�o�b�t�@�̐���
	DirectX11::GetInstance()->CreateBuffer(&m_pIndexBuffer, (void*)pmx.mesh.indices.data(), sizeof(int) * pmx.mesh.indices.size(), D3D11_BIND_INDEX_BUFFER);
}

WCHAR* Mesh::ToWCHAR(WCHAR* wc, const std::string & str)
{
	size_t	ret;
	setlocale(LC_ALL, "japanese");
	mbstowcs_s(&ret, wc, 100, str.c_str(), _TRUNCATE);
	return wc;
}
