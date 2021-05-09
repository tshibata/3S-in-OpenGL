#include <math.h>
#include <platform.h>
#include "ref.h"
#include "common.h"
#include "geometry.h"
#include "files.h"

RenderingMode background;
RenderingMode solid2D;
RenderingMode solid3D;
RenderingMode lucid3D;

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

void expand(float * mat, float sx, float sy, float sz)
{
	float mat1[16] = {
		sx, 0.0f, 0.0f, 0.0f,
		0.0f, sy, 0.0f, 0.0f,
		0.0f, 0.0f, sz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	prod(mat, mat1);
}

void para(float * mat, float width, float height, float near, float far)
{
	float mat1[16] = {
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (far - near), 0.0f,
		0.0f, 0.0f, - near / (far - near), 1.0f,
	};
	prod(mat, mat1);
}

void pers(float * mat, float width, float height, float near, float far)
{
	float mat1[16] = {
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (far - near), 1.0f / far,
		0.0f, 0.0f, - near / (far - near), 0.0f,
	};
	prod(mat, mat1);
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


Stop::Stop()
{
}

void Stop::getMatrix(float * matrix)
{
}

void Stop::getInvertedMatrix(float * matrix)
{
}


Direction::Direction()
{
}
Direction::~Direction()
{
}


Projection::Projection(float width, float height, float near, float far) : width(width), height(height), near(near), far(far)
{
}


Texture * Texture::last = nullptr;

Texture::Texture(const char * path) : path(path), id(nextId()), next(last)
{
	last = this;
}

Texture::Texture(int width, int height) : width(width), height(height), path(nullptr), id(nextId()), next(last)
{
	last = this;
}

int Texture::nextId()
{
	if (last == nullptr)
	{
		return 0;
	}
	else
	{
		return last->id + 1;
	}
}


Figure * Figure::last = nullptr;

Figure::Figure(Texture * texture) : texture(texture)
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


SpacialFigure::SpacialFigure(Texture * texture, const char * path) : Figure::Figure(texture), path(path)
{
}

void SpacialFigure::getData(unsigned char * * data, size_t * size)
{
	readBin(path, data, size);
}


SurficialFigure::SurficialFigure(Texture * texture, int x, int y, int left, int top, int right, int bottom) : Figure::Figure(texture), x(x), y(y), left(left), top(top), right(right), bottom(bottom)
{
}

void SurficialFigure::getData(unsigned char * * data, size_t * size)
{
	float * buf = (float *) malloc(48 * sizeof(float));
	* data = (unsigned char *) buf;
	* size = 48 * sizeof(float);

	float dstX1 = (2 * - left) / (float) screenWidth;
	float dstY1 = (2 * top) / (float) screenHeight;
	float dstX2 = (2 * right) / (float) screenWidth;
	float dstY2 = (2 * - bottom) / (float) screenHeight;
	float srcX1 = (x - left) / (float) texture->width;
	float srcY1 = (y - top) / (float) texture->height;
	float srcX2 = (x + right) / (float) texture->width;
	float srcY2 = (y + bottom) / (float) texture->height;

	int i = 0;

	buf[i++] = srcX1;	buf[i++] = srcY2;
	buf[i++] = dstX1;	buf[i++] = dstY2;	buf[i++] = 0.0f;
	buf[i++] = 0;	buf[i++] = 0;	buf[i++] = -1.0f;

	buf[i++] = srcX1;	buf[i++] = srcY1;
	buf[i++] = dstX1;	buf[i++] = dstY1;	buf[i++] = 0.0f;
	buf[i++] = 0;	buf[i++] = 0;	buf[i++] = -1.0f;

	buf[i++] = srcX2;	buf[i++] = srcY2;
	buf[i++] = dstX2;	buf[i++] = dstY2;	buf[i++] = 0.0f;
	buf[i++] = 0;	buf[i++] = 0;	buf[i++] = -1.0f;

	buf[i++] = srcX2;	buf[i++] = srcY2;
	buf[i++] = dstX2;	buf[i++] = dstY2;	buf[i++] = 0.0f;
	buf[i++] = 0;	buf[i++] = 0;	buf[i++] = -1.0f;

	buf[i++] = srcX1;	buf[i++] = srcY1;
	buf[i++] = dstX1;	buf[i++] = dstY1;	buf[i++] = 0.0f;
	buf[i++] = 0;	buf[i++] = 0;	buf[i++] = -1.0f;

	buf[i++] = srcX2;	buf[i++] = srcY1;
	buf[i++] = dstX2;	buf[i++] = dstY1;	buf[i++] = 0.0f;
	buf[i++] = 0;	buf[i++] = 0;	buf[i++] = -1.0f;
}


AbstractPresence::AbstractPresence() : mode(nullptr), prev(this), next(this)
{
}

AbstractPresence::AbstractPresence(RenderingMode & mode) : prev(mode.sentinel.prev), next(& mode.sentinel), mode(& mode)
{
	// FIXME make this thread safe
	next->prev = this;
	prev->next = this;
}

AbstractPresence::~AbstractPresence()
{
	// FIXME make this thread safe
	next->prev = prev;
	prev->next = next;
}

void AbstractPresence::getMatrix(float * matrix)
{
	getDirection()->getMatrix(matrix);
}

AbstractPresence * AbstractPresence::getNext()
{
	if (next == & mode->sentinel)
	{
		return nullptr;
	}
	else
	{
		return next;
	}
}

Direction * AbstractPresence::getDirection()
{
	return nullptr;
}

Figure * AbstractPresence::getFigure()
{
	return nullptr;
}


RenderingMode::RenderingMode() : sentinel(* this)
{
}

AbstractPresence * RenderingMode::getFirst()
{
	return sentinel.getNext();
}


Scene::Scene()
{
}

Scene::~Scene()
{
}

