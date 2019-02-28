#pragma once
#include <vector>
#include "../Reader/PMXFileReader.h"
#include "../Reader/PMDFileReader.h"
#include "../../util/math/Matrix.h"

class Animation;

class Skeleton
{
public:
	// ボーン構造体
	struct Bone {					// ボーン構造体
		char			name[32];	// ボーン名
		int				ID;			// ボーンID
		int				parentID;	// 親ボーンID
		Matrix			transform;	// ボーンの変換行列
		Matrix			inverse;	// ボーンの逆行列
	};

public:
	// コンストラクタ
	Skeleton();
	// デストラクタ
	~Skeleton();
	// PMDファイルの読み込み
	void Load(const mmd::PMD::PMDFile & pmd);
	// PMXファイルの読み込み
	void Load(const mmd::pmx::PMXFile & pmx);
	// ボーン数を返す
	std::size_t Size() const;
	// ボーンのローカル変換行列を計算する
	void CalculateLocalMatrices(const Animation& animation, float frameNo, Matrix local[]) const;
	// ボーンのワールド変換行列を計算する
	void CalculateWorldMatrices(const Matrix& parent, const Matrix local[], Matrix world[]) const;
	// スキニング用の変換行列を計算する
	void CalculateSkinnedMatrices(const Matrix world[], Matrix skinned[]) const;
	// 消去
	void Clear();

private:
	// ボーン配列
	std::vector<Bone>	m_Bones;
};