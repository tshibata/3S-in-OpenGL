extern GLuint vao;
extern GLuint * vbo;
extern GLuint * tex;

template <typename T> class DirectUniform
{
private:
	GLint location;
public:
	DirectUniform(GLuint program, const GLchar *name);
	void set(AbstractPresence<T> * b);
};
template <typename T> DirectUniform<T>::DirectUniform(GLuint program, const GLchar *name)
{
	location = glGetUniformLocation(program, name);
}
template <typename T> void DirectUniform<T>::set(AbstractPresence<T> * b)
{
	Matrix4x4 objectMatrix;
	b->getMatrix(objectMatrix.elements);
	glUniformMatrix4fv(location, 1, GL_FALSE, objectMatrix.elements);
}

template <typename T> class UniformMatrix
{
private:
	GLint location;
	float * matrix;
public:
	UniformMatrix(GLuint program, const GLchar *name, float * matrix);
	void set(AbstractPresence<T> * b);
};
template <typename T> UniformMatrix<T>::UniformMatrix(GLuint program, const GLchar *name, float * matrix) : matrix(matrix)
{
	location = glGetUniformLocation(program, name);
}
template <typename T> void UniformMatrix<T>::set(AbstractPresence<T> * b)
{
		GLfloat matrix2[16];
		Matrix4x4 objectMatrix;
		b->getMatrix(objectMatrix.elements); // FIXME optimize!!
		prod(objectMatrix.elements, matrix, matrix2);
		glUniformMatrix4fv(location, 1, GL_FALSE, matrix2);
}

template <typename T> class UniformTexture
{
private:
	GLint location;
public:
	UniformTexture(GLuint program, const GLchar *name);
	void set(AbstractPresence<T> * b);
};
template <typename T> UniformTexture<T>::UniformTexture(GLuint program, const GLchar *name)
{
	location = glGetUniformLocation(program, name);
}
template <typename T> void UniformTexture<T>::set(AbstractPresence<T> * b)
{
	glUniform1i(location, b->getFigure()->texture->id);
}

template <typename T> class VertexAttrib
{
private:
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const void * pointer;
public:
	VertexAttrib(GLuint program, GLuint index, const GLchar *name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
	~VertexAttrib();
	void set(AbstractPresence<T> * b);
};
template <typename T> VertexAttrib<T>::VertexAttrib(GLuint program, GLuint index, const GLchar *name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer) :
	index(index), size(size), type(type), normalized(normalized), stride(stride), pointer(pointer)
{
	glEnableVertexAttribArray(index);
	glBindAttribLocation(program, index, name);
}
template <typename T> VertexAttrib<T>::~VertexAttrib()
{
	glDisableVertexAttribArray(index);
}
template <typename T> void VertexAttrib<T>::set(AbstractPresence<T> * b)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[b->getFigure()->id]);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

class ShadowRenderer
{
private:
	GLuint program;
public:
	float * lightingMatrix;
	Texture * shadowMap;
	ShadowRenderer(Texture & shadowMap);
	~ShadowRenderer();
	void process();
};

class SolidRenderer
{
private:
	GLuint program;
public:
	float * framingMatrix;
	float * lightingMatrix;
	Texture * shadowMap;
	SolidRenderer(Texture & shadowMap);
	~SolidRenderer();
	void process();
};

class LucidRenderer
{
private:
	GLuint program;
public:
	float * framingMatrix;
	LucidRenderer();
	~LucidRenderer();
	void process();
};

class Solid2DRenderer
{
private:
	GLuint program;
public:
	Solid2DRenderer();
	~Solid2DRenderer();
	void process();
};

