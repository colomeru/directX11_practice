#pragma once
#include "../util/Singleton.h"
#include <string>
#include <unordered_map>
#include "../ID.h"
#include "ModelPtr.h"

class ModelManager : public Singleton<ModelManager>
{
public:
	// Singleton �ł̃C���X�^���X�쐬�͋���
	friend class Singleton<ModelManager>;

public:
	// �ǂݍ���
	bool Load(const std::string& fileName, const MODEL_ID& id);
	// �擾
	ModelPtr Get(MODEL_ID id) const;
	// �N���A
	void Clear();

protected:
	// �R���X�g���N�^
	ModelManager();
	// �f�X�g���N�^
	virtual ~ModelManager();

private:
	// ���f���t�H���_�K�w
	const std::string	PATH = "res/3D/";
	// ���f���Ǘ��}�b�v
	std::unordered_map<MODEL_ID, ModelPtr>	m_ModelMap;
};