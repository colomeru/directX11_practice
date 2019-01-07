#pragma once
//#include <wrl.h>					// Microsoft::WRL::ComPtr
//using namespace Microsoft::WRL;

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#include <atlbase.h>
#include "../util/Singleton.h"

class RenderTarget;
class DepthStencil;

class DirectX11 : public Singleton<DirectX11>
{
public:
	// Singleton �ł̃C���X�^���X�쐬�͋���
	friend class Singleton<DirectX11>;

public:
	// ������
	bool Initialize();
	// �����_�[�^�[�Q�b�g�̐ݒ�
	void SetRenderTarget(const RenderTarget* renderTarget, DepthStencil* depthStencil);
	// �����_�[�^�[�Q�b�g�̃N���A
	void ClearRenderTarget(const RenderTarget* renderTarget, float color[]);
	// �f�v�X�X�e���V���̃N���A
	void ClearDepthStencil(DepthStencil* depthStencil, float depth, float stencil);
	// �f�o�C�X�̎擾
	ID3D11Device* GetDevice() const;
	// �f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext* GetContext() const;
	// �X���b�v�`�F�C���̎擾
	IDXGISwapChain* GetSwapChain() const;

	// �o�b�t�@����
	bool CreateBuffer(ID3D11Buffer** ppBuffer, void* resources, size_t size, D3D11_BIND_FLAG bindFlag);
	// �o�b�t�@�̐���
	//bool CreateBuffer(ID3D11Buffer** ppBuffer, void* resources, size_t size, BufferType type);
	// �r���[�|�[�g�̍쐬
	void CreateViewPort();

	// ���X�^���C�U�̐ݒ�
	bool SetRasterizer(D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID, D3D11_CULL_MODE cullMode = D3D11_CULL_NONE);

	// �u�����h �X�e�[�g�𖳌��ɂ��邽�߂̐ݒ���擾����
	D3D11_RENDER_TARGET_BLEND_DESC GetNoBlendDesc();
	// ���`�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
	D3D11_RENDER_TARGET_BLEND_DESC GetAlignmentBlendDesc();
	// ���Z�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
	D3D11_RENDER_TARGET_BLEND_DESC GetAddBlendDesc();
	// ���Z�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
	D3D11_RENDER_TARGET_BLEND_DESC GetSubtractBlendDesc();
	// �ώZ�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
	D3D11_RENDER_TARGET_BLEND_DESC GetMultipleBlendDesc();
	// �u�����h�X�e�[�g��ݒ肷��
	bool SetBlendState(D3D11_RENDER_TARGET_BLEND_DESC BlendStateArray[], UINT NumBlendState, bool AlphaToCoverageEnable);
	//�A���t�@�u�����f�B���O�ݒ�
	void SetAlphaBlend(bool alphaEnable, bool alphaToCoverageEnable);

protected:
	// �R���e�L�X�g
	DirectX11();
	// ���z�f�X�g���N�^
	virtual ~DirectX11();

private:
	// �A�_�v�^�̍쐬
	bool CreateAdapter();
	// �f�o�C�X�ƃX���b�v�`�F�C���̍쐬
	bool CreateDeviceAndSwapChain();
	// �E�B���h�E�A�\�V�G�[�V�����̐ݒ�
	bool SetWindowAssociation();

private:
	D3D_DRIVER_TYPE			m_DriverType;		// �h���C�o�[�^�C�v
	D3D_FEATURE_LEVEL		m_FeatureLevel;		// �@�\���x��

	CComPtr<ID3D11Device>			m_pDevice;			// �f�o�C�X
	CComPtr<ID3D11DeviceContext>	m_pDeviceContext;	// �f�o�C�X�R���e�L�X�g
	CComPtr<IDXGISwapChain>			m_pSwapChain;		// �X���b�v�`�F�C��
	CComPtr<IDXGIAdapter>			m_pAdapter;			// �A�_�v�^
};


