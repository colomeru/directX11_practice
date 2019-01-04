#pragma once
#include "../util/Singleton.h"
#include <string>
#include <memory>
#include "vertex/VertexShader.h"
#include "pixel/PixelShader.h"
#include <unordered_map>
#include "../ID.h"

// ���_�V�F�[�_�[�̃V�F�A�[�h�|�C���^
using VSPtr = std::shared_ptr<VertexShader>;
// �s�N�Z���V�F�[�_�[�N���X�̃V�F�A�[�h�|�C���^
using PSPtr = std::shared_ptr<PixelShader>;

class ShaderManager : public Singleton<ShaderManager>
{
public:
	// Singleton �ł̃C���X�^���X�쐬�͋���
	friend class Singleton<ShaderManager>;

public:
	// ���_�V�F�[�_�̒ǉ�
	void Add(VERTEX_SHADER_ID id, const VSPtr shader);
	// �s�N�Z���V�F�[�_�̒ǉ�
	void Add(PIXEL_SHADER_ID id, const PSPtr shader);
	// ���_�V�F�[�_�̎擾
	VSPtr Get(const VERTEX_SHADER_ID id) const;
	// �s�N�Z���V�F�[�_�̎擾
	PSPtr Get(const PIXEL_SHADER_ID id) const;
	// �N���A
	void Clear();

protected:
	// �R���X�g���N�^
	ShaderManager();
	// �f�X�g���N�^
	virtual ~ShaderManager();

private:
	// ���_�V�F�[�_�}�b�v
	std::unordered_map<VERTEX_SHADER_ID, VSPtr>	m_VSUMap;
	// �s�N�Z���V�F�[�_�}�b�v
	std::unordered_map<PIXEL_SHADER_ID, PSPtr>	m_PSUMap;
};
