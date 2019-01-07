#pragma once
#include "../Math/Vector3.h"

struct BoundingSphere;
struct Line;
struct Ray;
struct OrientedBoundingBox;

// ���E�{�b�N�X�iAABB�j
struct BoundingBox
{
	Vector3		min;	// ���E���Ɋ܂܂��ŏ����W
	Vector3		max;	// ���E���Ɋ܂܂��ő���W
	
	// �R���X�g���N�^
	BoundingBox();
	// �R���X�g���N�^
	BoundingBox(const Vector3& min, const Vector3& max);

	// �_�������Ă��邩�H
	bool Contains(const Vector3& point) const;
	// ���E�{�b�N�X�������Ă��邩�H
	bool Contains(const BoundingBox& other) const;
	// ���E���������Ă��邩�H
	bool Contains(const BoundingSphere& sphere) const;
	// �����������Ă��邩�H
	bool Contains(const Line& line) const;
	// OBB�������Ă��邩�H
	bool Contains(const OrientedBoundingBox& obb) const;

	// ���E�{�b�N�X�Ƃ̌�������
	bool Intersects(const BoundingBox& other) const;
	// ���̂Ƃ̌�������
	bool Intersects(const BoundingSphere& sphere, Vector3* closest = 0) const;
	// �����Ƃ̌�������
	bool Intersects(const Line& line, Vector3* intersect = 0) const;
	// ���C�Ƃ̌�������
	bool Intersects(const Ray& ray, Vector3* intersect = 0) const;
	// �����Ƃ̌�������
	bool Intersects(const Vector3& position, const Vector3& direction, float &near, float &far) const;
	// 0BB�Ƃ̌�������
	bool Intersects(const OrientedBoundingBox& obb) const;

	// BoundingBox����^�[�Q�b�g�ɑ΂��Ĉ�ԋ߂����W��Ԃ�
	Vector3 ClosestPoint(const Vector3& target) const;

	// ���s�ړ�
	BoundingBox Translate(const Vector3& position) const;
	// �R�[�i�[�̍��W���擾
	Vector3 CornerPoint(int cornerIndex) const;
	// �T�C�Y�̎擾
	Vector3 Size() const;
	// ���S���W�̎擾
	Vector3 Center() const;
};
