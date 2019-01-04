#pragma once
#include "../util/Singleton.h"
#include <string>
#include <unordered_map>
#include "../ID.h"
#include "ModelPtr.h"

class ModelManager : public Singleton<ModelManager>
{
public:
	// Singleton でのインスタンス作成は許可
	friend class Singleton<ModelManager>;

public:
	// 読み込み
	bool Load(const std::string& fileName, const MODEL_ID& id);
	// 取得
	ModelPtr Get(MODEL_ID id) const;
	// クリア
	void Clear();

protected:
	// コンストラクタ
	ModelManager();
	// デストラクタ
	virtual ~ModelManager();

private:
	// モデルフォルダ階層
	const std::string	PATH = "res/3D/";
	// モデル管理マップ
	std::unordered_map<MODEL_ID, ModelPtr>	m_ModelMap;
};