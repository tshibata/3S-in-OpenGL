#include <math.h>
#include "geometry.h"

void prod(float * a, float * b, float * c)
{
	float c0 = a[0] * b[0] +  a[1] * b[4] +  a[2] * b[8] +  a[3] * b[12];
	float c1 = a[0] * b[1] +  a[1] * b[5] +  a[2] * b[9] +  a[3] * b[13];
	float c2 = a[0] * b[2] +  a[1] * b[6] +  a[2] * b[10] +  a[3] * b[14];
	float c3 = a[0] * b[3] +  a[1] * b[7] +  a[2] * b[11] +  a[3] * b[15];

	float c4 = a[4] * b[0] +  a[5] * b[4] +  a[6] * b[8] +  a[7] * b[12];
	float c5 = a[4] * b[1] +  a[5] * b[5] +  a[6] * b[9] +  a[7] * b[13];
	float c6 = a[4] * b[2] +  a[5] * b[6] +  a[6] * b[10] +  a[7] * b[14];
	float c7 = a[4] * b[3] +  a[5] * b[7] +  a[6] * b[11] +  a[7] * b[15];

	float c8 = a[8] * b[0] +  a[9] * b[4] +  a[10] * b[8] +  a[11] * b[12];
	float c9 = a[8] * b[1] +  a[9] * b[5] +  a[10] * b[9] +  a[11] * b[13];
	float c10 = a[8] * b[2] +  a[9] * b[6] +  a[10] * b[10] +  a[11] * b[14];
	float c11 = a[8] * b[3] +  a[9] * b[7] +  a[10] * b[11] +  a[11] * b[15];

	float c12 = a[12] * b[0] +  a[13] * b[4] +  a[14] * b[8] +  a[15] * b[12];
	float c13 = a[12] * b[1] +  a[13] * b[5] +  a[14] * b[9] +  a[15] * b[13];
	float c14 = a[12] * b[2] +  a[13] * b[6] +  a[14] * b[10] +  a[15] * b[14];
	float c15 = a[12] * b[3] +  a[13] * b[7] +  a[14] * b[11] +  a[15] * b[15];

	c[0] = c0;
	c[1] = c1;
	c[2] = c2;
	c[3] = c3;

	c[4] = c4;
	c[5] = c5;
	c[6] = c6;
	c[7] = c7;

	c[8] = c8;
	c[9] = c9;
	c[10] = c10;
	c[11] = c11;

	c[12] = c12;
	c[13] = c13;
	c[14] = c14;
	c[15] = c15;
}

void prod(float * a, float * b)
{
	prod(a, b, a);
}

void rotX(float * mat, float a)
{
	float mat1[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosf(a), -sinf(a), 0.0f,
		0.0f, sinf(a), cosf(a), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	prod(mat, mat1);
}

void rotY(float * mat, float a)
{
	float mat1[16] = {
		cosf(a), 0.0f, sinf(a), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinf(a), 0.0f, cosf(a), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	prod(mat, mat1);
}

void rotZ(float * mat, float a)
{
	float mat1[16] = {
		cosf(a), -sinf(a), 0.0f, 0.0f,
		sinf(a), cosf(a), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	prod(mat, mat1);
}

void move(float * mat, float x, float y, float z)
{
	float mat1[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f,
	};
	prod(mat, mat1);
}

void proj(float * mat, float width, float height, float depth)
{
	float mat1[16] = {
		1.0f / (width * depth), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / (height * depth), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / depth, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	prod(mat, mat1);
	float mat2[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, -1.0f, 1.0f,
	};
	prod(mat, mat2);
}

Matrix4x4::Matrix4x4()
{
	elements[0] = 1.0f;
	elements[1] = 0.0f;
	elements[2] = 0.0f;
	elements[3] = 0.0f;

	elements[4] = 0.0f;
	elements[5] = 1.0f;
	elements[6] = 0.0f;
	elements[7] = 0.0f;

	elements[8] = 0.0f;
	elements[9] = 0.0f;
	elements[10] = 1.0f;
	elements[11] = 0.0f;

	elements[12] = 0.0f;
	elements[13] = 0.0f;
	elements[14] = 0.0f;
	elements[15] = 1.0f;
}


class Stop : public Direction
{
public:
	static Stop singleton;
	Stop();
	virtual void getMatrix(float * matrix);
	virtual void getInvertedMatrix(float * matrix);
};

Stop Stop::singleton;

Stop::Stop()
{
}

void Stop::getMatrix(float * matrix)
{
}

void Stop::getInvertedMatrix(float * matrix)
{
}


Direction * Direction::stop = & Stop::singleton;

Direction::Direction()
{
}


Move::Move(Direction * next) : next(next)
{
}

void Move::getMatrix(float * matrix)
{
	move(matrix, dx, dy, dz);
	next->getMatrix(matrix);
}

void Move::getInvertedMatrix(float * matrix)
{
	next->getInvertedMatrix(matrix);
	move(matrix, - dx, - dy, - dz);
}


RotX::RotX(Direction * next) : next(next)
{
}

void RotX::getMatrix(float * matrix)
{
	rotX(matrix, angle);
	next->getMatrix(matrix);
}

void RotX::getInvertedMatrix(float * matrix)
{
	next->getInvertedMatrix(matrix);
	rotX(matrix, - angle);
}


RotY::RotY(Direction * next) : next(next)
{
}

void RotY::getMatrix(float * matrix)
{
	rotY(matrix, angle);
	next->getMatrix(matrix);
}

void RotY::getInvertedMatrix(float * matrix)
{
	next->getInvertedMatrix(matrix);
	rotY(matrix, - angle);
}


RotZ::RotZ(Direction * next) : next(next)
{
}

void RotZ::getMatrix(float * matrix)
{
	rotZ(matrix, angle);
	next->getMatrix(matrix);
}

void RotZ::getInvertedMatrix(float * matrix)
{
	next->getInvertedMatrix(matrix);
	rotZ(matrix, - angle);
}


Projection::Projection(Direction * direction) : direction(direction)
{
}

void Projection::getMatrix(float * matrix)
{
	direction->getInvertedMatrix(matrix);
	proj(matrix, width, height, depth);
}


Texture * Texture::last = nullptr;
Texture::Texture(const char * path) : path(path)
{
	if (last == nullptr)
	{
		id = 1;
	}
	else
	{
		id = last->id + 1;
	}
	next = last;
	last = this;
}

Figure * Figure::last = nullptr;
Figure::Figure(const char * path) : path(path)
{
	if (last == nullptr)
	{
		id = 0;
	}
	else
	{
		id = last->id + 1;
	}
	next = last;
	last = this;
}


class SentinelBeing : public Being
{
public:
	static SentinelBeing singleton;
	SentinelBeing();
	virtual BeingType getType();
	virtual Figure * getFigure();
	virtual Texture * getTexture();
};

SentinelBeing SentinelBeing::singleton;

SentinelBeing::SentinelBeing() : Being::Being(nullptr)
{
}

BeingType SentinelBeing::getType()
{
	return SOLID_BEING;
}

Figure * SentinelBeing::getFigure()
{
	return nullptr;
}

Texture * SentinelBeing::getTexture()
{
	return nullptr;
}


Being::Being(Direction * direction) : direction(direction)
{
	// FIXME make this thread safe
	next = & SentinelBeing::singleton;
	prev = SentinelBeing::singleton.prev;
	next->prev = this;
	prev->next = this;
	/*
	 * SentinelBeing::singleton is also initialilzed here.
	 * next = & SentinelBeing::singleton; // next = this
	 * prev = SentinelBeing::singleton.prev; // prev = prev // no matter
	 * next->prev = this; // prev = this
	 * prev->next = this; // next = this // again
	 */
}

Being::~Being()
{
	// FIXME make this thread safe
	next->prev = prev;
	prev->next = next;
}

Being * Being::getFirst()
{
	return SentinelBeing::singleton.next;
}

Being * Being::getNext()
{
	if (next == & SentinelBeing::singleton)
	{
		return nullptr;
	}
	return next;
}

void Being::getMatrix(float * matrix)
{
	direction->getMatrix(matrix);
}


Scene::Scene()
{
}
Scene::~Scene()
{
}

