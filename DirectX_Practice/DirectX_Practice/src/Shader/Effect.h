#pragma once
#include "../ID.h"
#include "ShaderManager.h"

class Effect
{
public:
	// �R���X�g���N�^
	Effect();
	// �R���X�g���N�^
	Effect(VERTEX_SHADER_ID vsID, PIXEL_SHADER_ID psID);
	// �R���X�g���N�^
	explicit Effect(VERTEX_SHADER_ID vsID);
	// �R���X�g���N�^
	explicit Effect(PIXEL_SHADER_ID psID);
	// �f�X�g���N�^
	~Effect();
	// �ǂݍ���
	void Load(VERTEX_SHADER_ID vsID, PIXEL_SHADER_ID psID);
	// �V�F�[�_�[�J�n
	void Begin() const;
	// �V�F�[�_�[�I��
	void End() const;

private:
	VSPtr		m_VSShader;	// ���_�V�F�[�_�[
	PSPtr		m_PSShader;	// �s�N�Z���V�F�[�_�[
};
