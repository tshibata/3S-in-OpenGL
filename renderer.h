extern GLuint vao;
extern GLuint * vbo;
extern GLsizei * vbSize;
extern GLuint * tex;

inline void setPresence(AbstractPresence * p)
{
}

template <class Head, class... Tail> void setPresence(AbstractPresence * p, Head&& head, Tail&&... tail)
{
	head.set(p);
	setPresence(p, std::forward<Tail>(tail)...);
}

template <class... Tail> void present(RenderingMode & mode, Tail... tail)
{
	for (AbstractPresence * p = mode.getFirst(); p != nullptr; p = p->getNext())
	{
		if (p->visible)
		{
			setPresence(p, std::forward<Tail>(tail)...);
			glDrawArrays(GL_TRIANGLES, 0, vbSize[p->getFigure()->id] / (8 * sizeof(float)));
		}
	}
}

class DirectUniform
{
private:
	GLint location;
public:
	DirectUniform(GLuint program, const GLchar *name)
	{
		location = glGetUniformLocation(program, name);
	}
	void set(AbstractPresence * b)
	{
		Matrix4x4 objectMatrix;
		b->getMatrix(objectMatrix.elements);
		glUniformMatrix4fv(location, 1, GL_FALSE, objectMatrix.elements);
	}
};

class UniformMatrix
{
private:
	GLint location;
	float * matrix;
public:
	UniformMatrix(GLuint program, const GLchar *name, float * matrix) : matrix(matrix)
	{
		location = glGetUniformLocation(program, name);
	}
	void set(AbstractPresence * b)
	{
		GLfloat matrix2[16];
		Matrix4x4 objectMatrix;
		b->getMatrix(objectMatrix.elements); // FIXME optimize!!
		prod(objectMatrix.elements, matrix, matrix2);
		glUniformMatrix4fv(location, 1, GL_FALSE, matrix2);
	}
};

class UniformTexture
{
private:
	GLint location;
public:
	UniformTexture(GLuint program, const GLchar *name)
	{
		location = glGetUniformLocation(program, name);
	}
	void set(AbstractPresence * b)
	{
		glUniform1i(location, b->getFigure()->texture->id);
	}
};

class VertexAttrib
{
private:
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const void * pointer;
public:
	VertexAttrib(GLuint program, GLuint index, const GLchar *name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer) :
	index(index), size(size), type(type), normalized(normalized), stride(stride), pointer(pointer)
	{
		glEnableVertexAttribArray(index);
		glBindAttribLocation(program, index, name);
	}
	~VertexAttrib()
	{
		glDisableVertexAttribArray(index);
	}
	void set(AbstractPresence * b)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[b->getFigure()->id]);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}
};

class StencilOperation
{
public:
	StencilOperation()
	{
	}
	void set(AbstractPresence * b)
	{
		glStencilOp(GL_KEEP, GL_KEEP, b->label ? GL_REPLACE : GL_KEEP);
		glStencilFunc(GL_ALWAYS, b->label, 0xFF);
	}
};

