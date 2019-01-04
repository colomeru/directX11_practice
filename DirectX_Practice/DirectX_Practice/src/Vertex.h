#pragma once
#include "util/math/Vector2.h"
#include "util/math/Vector3.h"
#include "util/math/Color.h"
#include "util/math/Matrix.h"

struct Vertex
{
	Vector3	Position;
	Vector3	Normal;
	//Color	Color;
	Vector2	Texcoord;
};

struct TextureVertex
{
	Vector3	Position;
	Vector2	Texcoord;
};

struct CharacterVertex
{
	Vector3 Position;
};

struct ShadowVertex
{
	Vector3 Position;
};

struct SkineedVertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector2	Texcoord;
	Matrix  BoneIndices;
	Matrix  BoneWeight;
};