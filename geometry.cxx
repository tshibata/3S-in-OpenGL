#include <stdio.h>
#include <math.h>
#include "geometry.h"

void init(float * matrix)
{
	matrix[0] = 1.0f;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = 1.0f;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = 1.0f;
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;
}

void prod(float * a, float * b)
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

	a[0] = c0;
	a[1] = c1;
	a[2] = c2;
	a[3] = c3;

	a[4] = c4;
	a[5] = c5;
	a[6] = c6;
	a[7] = c7;

	a[8] = c8;
	a[9] = c9;
	a[10] = c10;
	a[11] = c11;

	a[12] = c12;
	a[13] = c13;
	a[14] = c14;
	a[15] = c15;
}

void prod(float * a, float * b, float * c)
{
	c[0] = a[0] * b[0] +  a[1] * b[4] +  a[2] * b[8] +  a[3] * b[12];
	c[1] = a[0] * b[1] +  a[1] * b[5] +  a[2] * b[9] +  a[3] * b[13];
	c[2] = a[0] * b[2] +  a[1] * b[6] +  a[2] * b[10] +  a[3] * b[14];
	c[3] = a[0] * b[3] +  a[1] * b[7] +  a[2] * b[11] +  a[3] * b[15];

	c[4] = a[4] * b[0] +  a[5] * b[4] +  a[6] * b[8] +  a[7] * b[12];
	c[5] = a[4] * b[1] +  a[5] * b[5] +  a[6] * b[9] +  a[7] * b[13];
	c[6] = a[4] * b[2] +  a[5] * b[6] +  a[6] * b[10] +  a[7] * b[14];
	c[7] = a[4] * b[3] +  a[5] * b[7] +  a[6] * b[11] +  a[7] * b[15];

	c[8] = a[8] * b[0] +  a[9] * b[4] +  a[10] * b[8] +  a[11] * b[12];
	c[9] = a[8] * b[1] +  a[9] * b[5] +  a[10] * b[9] +  a[11] * b[13];
	c[10] = a[8] * b[2] +  a[9] * b[6] +  a[10] * b[10] +  a[11] * b[14];
	c[11] = a[8] * b[3] +  a[9] * b[7] +  a[10] * b[11] +  a[11] * b[15];

	c[12] = a[12] * b[0] +  a[13] * b[4] +  a[14] * b[8] +  a[15] * b[12];
	c[13] = a[12] * b[1] +  a[13] * b[5] +  a[14] * b[9] +  a[15] * b[13];
	c[14] = a[12] * b[2] +  a[13] * b[6] +  a[14] * b[10] +  a[15] * b[14];
	c[15] = a[12] * b[3] +  a[13] * b[7] +  a[14] * b[11] +  a[15] * b[15];
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


