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
	// �}�e���A���\����
	struct Material
	{
		Color 			ambient;						// �����J���[
		Color 			diffuse;						// �g�U���ˌ��J���[
		Color 			specular;						// ���ʔ��ˌ��J���[
		Color 			emission;						// ���ȏƖ��J���[
		float			shininess;						// ���ʔ��ˎw��
		CComPtr<ID3D11ShaderResourceView>	pTexture;	// �e�N�X�`��
		D3D11_CULL_MODE	cullMode;						// �`��J�����O���[�h
	};
	// �T�u�Z�b�g�\����
	struct Subset
	{
		int						material;		// �}�e���A��
		int						start;			// �C���f�N�X�̊J�n�ʒu
		int						count;			// �C���f�b�N�X��
	};
	// ���b�V���V�F�[�_�[�N���X
	class Shader 
	{
	public:
		// ���z�f�X�g���N�^
		virtual ~Shader() {}
		// �J�n
		virtual void Begin() = 0;
		// �I��
		virtual void End() = 0;
		// �}�e���A���̐ݒ�
		virtual void Material(const Mesh::Material& material, ID3D11Buffer* constantBuffer, ID3D11SamplerState* sampler) = 0;
	};

public:
	// �R���X�g���N�^
	Mesh();
	// �f�X�g���N�^
	~Mesh();
	// �`��
	void Draw(Mesh::Shader& shader) const;
	// PMD�t�@�C���̓ǂݍ���
	void Load(const mmd::PMD::PMDFile & pmd, const std::string & folderPATH);
	// PMX�t�@�C���̓ǂݍ���
	void Load(const mmd::pmx::PMXFile & pmx, const std::string & folderPATH);
	
	// �R�s�[�֎~
	//Mesh(const Mesh&) = delete;
	//Mesh& operator = (const Mesh&) = delete;

private:
	//
	WCHAR* ToWCHAR(WCHAR* wc, const std::string & str);
	// ���_�o�b�t�@�̐���
	void CreateVertexBuffer(void * resources, int size);
	// �C���f�b�N�X�o�b�t�@�̐���
	void CreateIndexBuffer(void * resources, int size);

	// PMD_���_���X�g�̓ǂݍ���
	void LoadVertices(const mmd::PMD::Vertices& pmdvertices);
	// PMX_���_���X�g�̓ǂݍ���
	void LoadVertices(const mmd::pmx::Vertices& pmxVertices);
	// ���_�f�[�^�̃Z�b�g
	void SetVertexParameter(Vertex & vertex, std::function<void(Vertex&)> func);

private:
	// �}�e���A���z��
	std::vector<Material>	m_Materials;
	// �T�u�Z�b�g�z��
	std::vector<Subset>		m_Subsets;

	CComPtr<ID3D11Buffer>			m_pVertexBuffer;	// ���_�o�b�t�@
	CComPtr<ID3D11Buffer>			m_pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	CComPtr<ID3D11Buffer>			m_pConstantBuffer;	// �R���X�g���N�^�o�b�t�@
	CComPtr<ID3D11SamplerState>		m_pSampler;			// �T���v���[
};