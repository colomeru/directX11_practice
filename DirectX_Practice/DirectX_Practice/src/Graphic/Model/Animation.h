#pragma once
#include <string>
#include <unordered_map>
#include <string>
#include "../../util/math/Vector3.h"
#include "../../util/math/Quaternion.h"
#include "../../util/math/Matrix.h"
#include <map>
#include "../Reader/VMDFileReader.h"

// ボーンフレーム
using BoneFrame = std::map<std::string, mmd::vmd::Motions>;

class Animation
{
public:
	struct KeyFrame						// キーフレーム構造体
	{
		float			frameNo;		// フレーム番号
		Vector3			scale;			// 拡大縮小
		Quaternion		rotate;			// 回転
		Vector3			position;		// 平行移動
		Matrix			matrix() const;	// 変換行列を計算する

		explicit KeyFrame() = default;
	};

public:
	// コンストラクタ
	Animation();
	// デストラクタ
	~Animation();
	// 読み込み
	void Load(const std::string& fileName);
	// キーフレームの取得
	Animation::KeyFrame GetKeyFrame(const std::string& boneName, float frameNo) const;
	// 終了フレーム数を返す
	float EndFrame() const;
	// 消去
	void Clear();

private:
	// ボーンフレームの生成
	BoneFrame CreateBoneFrame(const mmd::vmd::VMDFile& vmd);
	// キーフレームの読み込み
	void LoadBoneKeyFrame(const BoneFrame& boneFrame);

private:
	// ボーンキーフレーム
	std::unordered_map<std::string, std::vector<KeyFrame>> m_BoneKeyFrame;

};
