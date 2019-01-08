#pragma once
#include "../util/Singleton.h"
#include <d3d11.h>
#include <atlbase.h>
#include <string>
#include <unordered_map>
#include "../ID.h"
#include "SpritePtr.h"
#include "../Shader/Effect.h"

struct Vector2;
struct ShaderResource;
class RenderTexture;

class SpriteManager : public Singleton<SpriteManager>
{
public:
	//
	friend class Singleton<SpriteManager>;

public:
	// ������
	void Initialize(int width, int height);
	// �ǂݍ���
	bool Load(const std::string& fileName, const SPRITE_ID& id);
	// �N���A
	void Clear();

	// �`��i���炩���߃��\�[�X�f�[�^�͑��M���Ă����j
	void DrawGraph(Vector2 position, Vector2 size);
	// �`��i���炩���߃��\�[�X�f�[�^�͑��M���Ă����j
	void DrawGraph(float x, float y, unsigned int width, unsigned int height);
	// �`��
	void DrawGraph(Vector2 position, ID3D11ShaderResourceView* srv, ID3D11SamplerState* sampler, unsigned int width, unsigned int height);
	// �`��
	void DrawGraph(Vector2 position, const ShaderResource& res, unsigned int width, unsigned int height);
	// �`��
	void DrawGraph(Vector2 position, const RenderTexture& renderTexture);

	// �`��
	void Draw(const SpritePtr& sprite, Vector2 position, Vector2 size);
	// �`��
	void Draw(SPRITE_ID id, Vector2 position);
	// �`��
	void Draw(SPRITE_ID id, Vector2 position, Vector2 size);

protected:
	// �R���X�g���N�^
	SpriteManager();
	// �f�X�g���N�^
 	virtual ~SpriteManager();

private:
	// ���_�f�[�^�𐶐����V�F�[�_�ɑ���
	void SetVertexBuffer();
	// �萔�o�b�t�@�𐶐����V�F�[�_�ɑ���
	void SetConstantBuffer(unsigned int width, unsigned int height, Vector2 position);

private:
	const std::string PATH = "res/2D/";
	
	std::unordered_map<SPRITE_ID, SpritePtr>	m_SpriteMap;

	CComPtr<ID3D11Buffer>		m_pVertexBuffer;	// ���_�o�b�t�@
	CComPtr<ID3D11Buffer>		m_pConstantBuffer;	// �萔�o�b�t�@

	unsigned int				m_Width;			// �E�B���h�E�̏c��
	unsigned int				m_Height;			// �E�B���h�E�̉���

	Effect						m_Effect;
};
