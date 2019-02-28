#pragma once
#include "../DirectX/ShaderResource.h"
#include <string>

class Texture;

class Sprite
{
public:
	// �R���X�g���N�^
	Sprite();
	// �f�X�g���N�^
	~Sprite();
	// �`��
	void Begin() const;
	// �I��
	void End();
	// �e�N�X�`�����琶��
	bool Create(const std::string& filePath);
	// �e�N�X�`�����琶��
	bool Create(const Texture* pTexture);

	// �����擾
	unsigned int GetWidth() const;
	// �c���擾
	unsigned int GetHeight() const;
	// �V�F�[�_�[���\�[�X�擾
	ShaderResource GetShaderResource() const;

private:
	unsigned int						m_Width;	// ����
	unsigned int						m_Height;	// �c��
	ShaderResource						m_Res;		// ���\�[�X
};