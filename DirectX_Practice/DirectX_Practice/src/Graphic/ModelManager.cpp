#include "ModelManager.h"
#include "Model.h"

ModelManager::ModelManager()
{
	Clear();
}

ModelManager::~ModelManager()
{
	Clear();
}

bool ModelManager::Load(const std::string & fileName, const MODEL_ID& id)
{
	// �t�H���_
	int path_i = fileName.find_last_of("/") + 1;
	std::string	folder = fileName.substr(0, path_i);

	// �g���q�̎擾
	int ext_i = fileName.find_last_of(".");
	std::string extname = fileName.substr(ext_i, fileName.size() - ext_i);

	// �g���q�ɂ���ēǂݍ��ݕ��@��ς���
	if (extname == ".pmx")
	{
		// PMX�t�@�C���̓ǂݍ���
		mmd::pmx::PMXFile pmx;
		mmd::pmx::PMXFileReader(pmx).read(PATH + fileName);

		// �}�b�v�Ƀ��f���f�[�^��o�^
		m_ModelMap[id] = std::make_shared<Model>(pmx, PATH + folder);
	}
	else if (extname == ".pmd")
	{
		// PMD�t�@�C���̓ǂݍ���
		mmd::PMD::PMDFile pmd;
		mmd::PMD::PMDFileReader(pmd).read(PATH + fileName);

		// �}�b�v�Ƀ��f���f�[�^��o�^
		m_ModelMap[id] = std::make_shared<Model>(pmd, PATH + folder);;
	}

	return true;
}

ModelPtr ModelManager::Get(MODEL_ID id) const
{
	return m_ModelMap.at(id);
}

void ModelManager::Clear()
{
	m_ModelMap.clear();
}
