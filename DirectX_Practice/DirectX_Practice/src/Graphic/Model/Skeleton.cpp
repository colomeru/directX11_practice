#include "Skeleton.h"
#include "../util/math/Vector3.h"

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
	for (int i = 0; i < m_Bones.size(); i++)
	{
		//for (int j = 0; j < sizeof(pmx.skeleton[i].name); j++)
		//	m_Bones[i].name[j] = pmx.skeleton[i].name[j];
		//m_Bones[i].ID = pmx.skeleton[i].
		m_Bones[i].parentID = pmx.skeleton[i].parent_index;
		auto position = pmx.skeleton[i].position;
		m_Bones[i].transform = Matrix::CreateTranslation(Vector3(position.x, position.y, position.z));
		m_Bones[i].inverse   = Matrix::Invert(m_Bones[i].transform);
	}
}
