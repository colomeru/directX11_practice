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
	// フォルダ
	int path_i = fileName.find_last_of("/") + 1;
	std::string	folder = fileName.substr(0, path_i);

	// 拡張子の取得
	int ext_i = fileName.find_last_of(".");
	std::string extname = fileName.substr(ext_i, fileName.size() - ext_i);

	// 拡張子によって読み込み方法を変える
	if (extname == ".pmx")
	{
		// PMXファイルの読み込み
		mmd::pmx::PMXFile pmx;
		mmd::pmx::PMXFileReader(pmx).read(PATH + fileName);

		// マップにモデルデータを登録
		m_ModelMap[id] = std::make_shared<Model>(pmx, PATH + folder);
	}
	else if (extname == ".pmd")
	{
		// PMDファイルの読み込み
		mmd::PMD::PMDFile pmd;
		mmd::PMD::PMDFileReader(pmd).read(PATH + fileName);

		// マップにモデルデータを登録
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
