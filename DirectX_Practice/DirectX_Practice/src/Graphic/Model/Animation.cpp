#include "Animation.h"
#include <algorithm>

// キーフレームの区間を検索
inline std::pair<int, int> FindKeyFrameSegment(const std::vector<Animation::KeyFrame>& keys, float frameNo);

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::Load(const std::string & fileName)
{
	//
	mmd::vmd::VMDFile vmd;
	mmd::vmd::VMDFileReader(vmd).read(fileName);

	const auto& boneFrame = CreateBoneFrame(vmd);
	
	LoadBoneKeyFrame(boneFrame);

	auto a = 0;
}

Animation::KeyFrame Animation::GetKeyFrame(const std::string & boneName, float frameNo) const
{
	const auto keys = m_BoneKeyFrame.find(boneName);
	KeyFrame result;
	if (keys == m_BoneKeyFrame.end())
	{	// アニメーションキーが存在しない場合
		result.frameNo = frameNo;
		result.scale    = Vector3(1.0f, 1.0f, 1.0f);
		result.rotate   = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		result.position = Vector3(0.0f, 0.0f, 0.0f);
		return result;
	}
	const auto& segment = FindKeyFrameSegment(keys->second, frameNo);
	if (segment.first == segment.second)
		// アニメーションキーが一致
		return keys->second[segment.first];
	// アニメーションを補間
	const auto& first  = keys->second[segment.first];
	const auto& second = keys->second[segment.second];
	const float t = (frameNo - first.frameNo) / (second.frameNo - first.frameNo);
	result.frameNo = frameNo;
	result.scale = first.scale.Lerp(first.scale, second.scale, t);
	result.rotate = first.rotate.Slerp(first.rotate, second.rotate, t);
	result.position = first.position.Lerp(first.position, second.position, t);
	return result;
}

float Animation::EndFrame() const
{
	float result = 0.0f;
	for (const auto& keys : m_BoneKeyFrame)
	{
		result = std::max(result, keys.second.back().frameNo);
	}
	return result;
}

void Animation::Clear()
{
	m_BoneKeyFrame.clear();
}

BoneFrame Animation::CreateBoneFrame(const mmd::vmd::VMDFile & vmd)
{
	BoneFrame boneFrame;
	// ボーン名ごとのキーフレームを作成
	for (const auto& motion : vmd.motions)
		boneFrame[motion.bone_name].push_back(motion);

	// フレーム番号順にソート
	for (auto& keyFrame : boneFrame)
	{
		std::sort(keyFrame.second.begin(), keyFrame.second.end(),
			[](auto&& m1, auto&& m2) { return m1.frame_no < m2.frame_no; });
	}

	return boneFrame;
}

void Animation::LoadBoneKeyFrame(const BoneFrame & boneFrame)
{
	for (auto& bone : boneFrame)
	{
		m_BoneKeyFrame[bone.first].clear();
		for (int ki = 0; ki < bone.second.size(); ki++)
		{
			auto position = bone.second.at(ki).location;
			auto rotation = bone.second.at(ki).rotation;

			KeyFrame keyFrame;
			keyFrame.frameNo = bone.second.at(ki).frame_no * 2;	// 60fps
			keyFrame.scale = Vector3::One;
			keyFrame.position = Vector3(position.x, position.y, position.z);
			keyFrame.rotate = Quaternion(rotation.x, rotation.y, rotation.z, rotation.w);

			m_BoneKeyFrame[bone.first].push_back(keyFrame);
		}
	}
}

// キーフレームの区間を検索
inline std::pair<int, int> FindKeyFrameSegment(const std::vector<Animation::KeyFrame>& keys, float frameNo)
{
	int start = 0;
	int end = (int)keys.size() - 1;
	if (keys[start].frameNo >= frameNo)
		return std::pair<int, int>(start, start);
	if (keys[end].frameNo <= frameNo)
		return std::pair<int, int>(end, end);
	while ((start + 1) < end)
	{
		const int mid = (start + end) / 2;
		if (keys[mid].frameNo == frameNo)
			return std::pair<int, int>(mid, mid);
		else if (keys[mid].frameNo < frameNo)
			start = mid;
		else
			end = mid;
	}
	return std::pair<int, int>(start, end);
}

Matrix Animation::KeyFrame::matrix() const
{
	return Matrix::CreateScale(scale)
		 * Matrix::CreateFromQuaternion(rotate)
		 * Matrix::CreateTranslation(position);
}
