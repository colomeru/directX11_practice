#pragma once
#include <d3d11.h>

class GameBase
{
public:
	// �R���X�g���N�^
	GameBase();
	// �f�X�g���N�^
	~GameBase();

	// ���s
	bool Run(HINSTANCE hIns);

private:
	// ���\�[�X�ǂݍ���
	void LoadResources();

private:
	D3D_DRIVER_TYPE				m_DriverType;				// �h���C�o�[�^�C�v
	D3D_FEATURE_LEVEL			m_FeatureLevel;				// �@�\���x��
	UINT						m_MultiSampleCount;			// �}���`�T���v���̃J�E���g
	UINT						m_MultiSampleQuality;		// �}���`�T���v���̕i��
	UINT						m_MultiSampleMaxQuality;	// �}���`�T���v���̍ő�i��
	UINT						m_SwapChainCount;			// �X���b�v�`�F�C���̃J�E���g
	DXGI_FORMAT					m_SwapChainFormat;			// �X���b�v�`�F�C���̃t�H�[�}�b�g
	DXGI_FORMAT					m_DepthStencilFormat;		// �[�x�X�e���V���̃t�H�[�}�b�g
	UINT						m_Width;					// �E�B���h�E����
	UINT						m_Height;					// �E�B���h�E�c��
	FLOAT						m_AspectRatio;				// �A�X�y�N�g��
};
