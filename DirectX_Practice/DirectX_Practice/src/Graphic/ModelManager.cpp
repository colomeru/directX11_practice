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
	auto model = std::make_shared<Model>();

	// フォルダ
	int path_i = fileName.find_last_of("/") + 1;
	std::string	folder = fileName.substr(0, path_i);

	// 拡張子の取得
	int ext_i = fileName.find_last_of(".");
	std::string extname = fileName.substr(ext_i, fileName.size() - ext_i);

	// 拡張子によって読み込み方法を変える
	if (extname == ".pmd")
	{
		// PMDファイルの読み込み
		mmd::PMD::PMDFile pmd;
		mmd::PMD::PMDFileReader(pmd).read(PATH + fileName);
		
		//
		model->mesh.Load(pmd, PATH + folder);
		model->skeleton.Load(pmd);
	}
	else if (extname == ".pmx")
	{
		// PMXファイルの読み込み
		mmd::pmx::PMXFile pmx;
		mmd::pmx::PMXFileReader(pmx).read(PATH + fileName);

		//
		model->mesh.Load(pmx, PATH + folder);
		model->skeleton.Load(pmx);
	}

	// マップにモデルデータを登録
	m_ModelMap[id] = model;
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
