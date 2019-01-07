#pragma once
#include "../Math/Vector3.h"

struct BoundingSphere;
struct BoundingBox;
struct Line;
struct Ray;
struct Matrix;

// �w�������E�{�b�N�X�iOBB�j
struct OrientedBoundingBox
{
	Vector3		position;	// ���S�̍��W
	Vector3		extents;	// �e���̃T�C�Y
	Vector3		axis[3];	// �e���̌���
	
	// �R���X�g���N�^
	OrientedBoundingBox();
	// �R���X�g���N�^
	OrientedBoundingBox(const Vector3& position, const Vector3& extents,
						const Vector3& axis_x, const Vector3& axis_y, const Vector3& axis_z);
	// �R���X�g���N�^
	explicit OrientedBoundingBox(const BoundingBox& aabb);

	// �_�������Ă��邩�H
	bool Contains(const Vector3& point) const;
	// ���E�{�b�N�X�������Ă��邩�H
	bool Contains(const BoundingBox& aabb) const;
	// ���E���������Ă��邩�H
	bool Contains(const BoundingSphere& sphere) const;
	// �����������Ă��邩�H
	bool Contains(const Line& line) const;
	// �w�������E�{�b�N�X�������Ă��邩�H
	bool Contains(const OrientedBoundingBox& other) const;

	// OBB�̌�������
	bool Intersects(const OrientedBoundingBox& other, float epsilon = 1e-3f) const;
	// AABB�̌�������
	bool Intersects(const BoundingBox& aabb, float epsilon = 1e-3f) const;
	// ���̂̌�������
	bool Intersects(const BoundingSphere& sphere, Vector3* closest = 0) const;
	// �����̌�������
	bool Intersects(const Line& line, Vector3* intersect = 0) const;
	// ���C�̌�������
	bool Intersects(const Ray& ray, Vector3* intersect = 0) const;
	// �����Ƃ̌�������
	bool Intersects(const Vector3& position, const Vector3& direction, float &near, float &far) const;

	// OBB����^�[�Q�b�g�ɑ΂��Ĉ�ԋ߂����W��Ԃ�
	Vector3 ClosestPoint(const Vector3& target) const;

	// ���W�ϊ�
	OrientedBoundingBox Transform(const Matrix& matrix) const;
	// ���[���h���W�ɕϊ�����s����쐬
	Matrix LocalToWorld() const;
	// ���[�J�����W�ɕϊ�����s����쐬
	Matrix WorldToLocal() const;

	// �R�[�i�[�̍��W���擾
	Vector3 CornerPoint(int cornerIndex) const;
	// �T�C�Y���擾
	Vector3 Size() const;
	// ���S���W���擾
	Vector3 Center() const;
};