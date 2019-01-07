#include "Line.h"

// �R���X�g���N�^
Line::Line() :
start(0.0f, 0.0f, 0.0f),
end(0.0f, 0.0f, 0.0f)
{
}

// �R���X�g���N�^
Line::Line(const Vector3 & start, const Vector3 & end) :
start(start),
end(end)
{
}

// ���������߂�
float Line::Length() const
{
	return Vector3::Distance(start, end);
}

// �n�_����I�_�ւ̕�����Ԃ�
Vector3 Line::Direction() const
{
	return Vector3::Normalize(end - start);
}

// ���W�ϊ�
Line Line::Transform(const Matrix & matrix) const
{
	return Line(Vector3::Transform(start, matrix), Vector3::Transform(end, matrix));
}
