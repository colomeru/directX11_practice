#include "Skeleton.h"
#include "../../util/math/Vector3.h"
#include "../../Graphic/Reader/MMDMath.h"
#include "Animation.h"

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
}

void Skeleton::Load(const mmd::PMD::PMDFile & pmd)
{
	m_Bones.resize(pmd.skeleton.bones.size());
	for (int i = 0; i < m_Bones.size(); i++)
	{
		for (int j = 0; j < sizeof(pmd.skeleton.bones[i].bone_name); j++)
			m_Bones[i].name[j] = pmd.skeleton.bones[i].bone_name[j];
		m_Bones[i].ID		= pmd.skeleton.bones[i].tail_bone_index;
		m_Bones[i].parentID = pmd.skeleton.bones[i].parent_bone_index;
		auto position = pmd.skeleton.bones[i].bone_pos;
		m_Bones[i].transform = Matrix::CreateTranslation(Vector3(position.x, position.y, position.z));
		m_Bones[i].inverse	 = Matrix::Invert(m_Bones[i].transform);
	}
}

void Skeleton::Load(const mmd::pmx::PMXFile & pmx)
{
	m_Bones.resize(pmx.skeleton.size());
	for (int id = 0; id < m_Bones.size(); id++)
	{
		const auto& bone = pmx.skeleton[id];
		const mmd::float3 parentOffset = (bone.parent_index == -1) ? bone.position : (bone.position - pmx.skeleton[bone.parent_index].position);
		strcpy_s(m_Bones[id].name, utf16_to_sjis(bone.name).c_str());
		m_Bones[id].ID = id;
		m_Bones[id].parentID  = bone.parent_index;
		m_Bones[id].transform = Matrix::CreateTranslation(Vector3(parentOffset.x, parentOffset.y, -parentOffset.z));
		m_Bones[id].inverse = Matrix::CreateTranslation(Vector3(-bone.position.x, -bone.position.y, -(-bone.position.z)));
		//m_Bones[id].transform = Matrix::CreateTranslation(Vector3(parentOffset.x, parentOffset.y, parentOffset.z));
		//m_Bones[id].inverse	  = Matrix::Invert(Matrix::CreateTranslation(Vector3(bone.position.x, bone.position.y, bone.position.z)));
	}
}

std::size_t Skeleton::Size() const
{
	return m_Bones.size();
}

void Skeleton::CalculateLocalMatrices(const Animation & animation, float frameNo, Matrix local[]) const
{
	for (const auto& bone : m_Bones)
	{
		auto m = animation.GetKeyFrame(bone.name, frameNo).matrix();
		local[bone.ID] = m;
	}
}

void Skeleton::CalculateWorldMatrices(const Matrix & parent, const Matrix local[], Matrix world[]) const
{
	world[0] = local[0] * m_Bones[0].transform * parent;
	for (int i = 1; i < (int)m_Bones.size(); ++i)
		world[i] = local[i] * m_Bones[i].transform * world[m_Bones[i].parentID];
	auto a = 0;
}

void Skeleton::CalculateSkinnedMatrices(const Matrix world[], Matrix skinned[]) const
{
	for (const auto& bone : m_Bones)
		skinned[bone.ID] = bone.inverse * world[bone.ID];
}

void Skeleton::Clear()
{
	m_Bones.clear();
}
