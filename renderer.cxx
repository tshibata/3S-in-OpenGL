#include <stdlib.h>
#include <stdio.h>
#include <platform.h>
#include "ref.h"
#include "geometry.h"
#include "common.h"
#include "renderer.h"

DirectUniform::DirectUniform(GLuint program, const GLchar *name)
{
	location = glGetUniformLocation(program, name);
}

void DirectUniform::set(AbstractPresence * b)
{
	Matrix4x4 objectMatrix;
	b->getMatrix(objectMatrix.elements);
	glUniformMatrix4fv(location, 1, GL_FALSE, objectMatrix.elements);
}

UniformMatrix::UniformMatrix(GLuint program, const GLchar *name, float * matrix) : matrix(matrix)
{
	location = glGetUniformLocation(program, name);
}
void UniformMatrix::set(AbstractPresence * b)
{
		GLfloat matrix2[16];
		Matrix4x4 objectMatrix;
		b->getMatrix(objectMatrix.elements); // FIXME optimize!!
		prod(objectMatrix.elements, matrix, matrix2);
		glUniformMatrix4fv(location, 1, GL_FALSE, matrix2);
}

UniformTexture::UniformTexture(GLuint program, const GLchar *name)
{
	location = glGetUniformLocation(program, name);
}
void UniformTexture::set(AbstractPresence * b)
{
	glUniform1i(location, b->getFigure()->texture->id);
}

VertexAttrib::VertexAttrib(GLuint program, GLuint index, const GLchar *name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer) :
	index(index), size(size), type(type), normalized(normalized), stride(stride), pointer(pointer)
{
	glEnableVertexAttribArray(index);
	glBindAttribLocation(program, index, name);
}
VertexAttrib::~VertexAttrib()
{
	glDisableVertexAttribArray(index);
}
void VertexAttrib::set(AbstractPresence * b)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[b->getFigure()->id]);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

StencilOperation::StencilOperation()
{
}
void StencilOperation::set(AbstractPresence * b)
{
	glStencilOp(GL_KEEP, GL_KEEP, b->label ? GL_REPLACE : GL_KEEP);
	glStencilFunc(GL_ALWAYS, b->label, 0xFF);
}

