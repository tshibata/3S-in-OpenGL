#include <stdio.h>
#include <math.h>
#include "geometry.h"

void initMatrix(float * matrix)
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

void matProd(float * a, float * b, float * c)
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
	float mat2[16];
	matProd(mat, mat1, mat2);
	for (int i = 0; i < 16; i++)
	{
		mat[i] = mat2[i];
	}
}

void rotY(float * mat, float a)
{
	float mat1[16] = {
		cosf(a), 0.0f, sinf(a), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinf(a), 0.0f, cosf(a), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	float mat2[16];
	matProd(mat, mat1, mat2);
	for (int i = 0; i < 16; i++)
	{
		mat[i] = mat2[i];
	}
}

void rotZ(float * mat, float a)
{
	float mat1[16] = {
		cosf(a), -sinf(a), 0.0f, 0.0f,
		sinf(a), cosf(a), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	float mat2[16];
	matProd(mat, mat1, mat2);
	for (int i = 0; i < 16; i++)
	{
		mat[i] = mat2[i];
	}
}

void move(float * mat, float x, float y, float z)
{
	float mat1[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f,
	};
	float mat2[16];
	matProd(mat, mat1, mat2);
	for (int i = 0; i < 16; i++)
	{
		mat[i] = mat2[i];
	}
}

void proj(float * mat, float width, float height, float depth)
{
	float mat1[16] = {
		1.0f / (width * depth), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / (height * depth), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / depth, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	float mat2[16];
	matProd(mat, mat1, mat2);
	float mat3[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, -1.0f, 1.0f,
	};
	float mat4[16];
	matProd(mat2, mat3, mat4);
	for (int i = 0; i < 16; i++)
	{
		mat[i] = mat4[i];
	}
}

void xyz2abc(float * xyz, float * abc, int offset)
{
	float dx1 = xyz[offset + 3] - xyz[offset + 0];
	float dx2 = xyz[offset + 6] - xyz[offset + 0];
	float dy1 = xyz[offset + 4] - xyz[offset + 1];
	float dy2 = xyz[offset + 7] - xyz[offset + 1];
	float dz1 = xyz[offset + 5] - xyz[offset + 2];
	float dz2 = xyz[offset + 8] - xyz[offset + 2];
	abc[offset + 0] = abc[offset + 3] = abc[offset + 6] = -(dy1 * dz2 - dz1 * dy2);
	abc[offset + 1] = abc[offset + 4] = abc[offset + 7] = -(dz1 * dx2 - dx1 * dz2);
	abc[offset + 2] = abc[offset + 5] = abc[offset + 8] = -(dx1 * dy2 - dy1 * dx2);	
}

