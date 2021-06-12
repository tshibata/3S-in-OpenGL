extern GLuint vao;
extern GLuint * vbo;
extern GLsizei * vbSize;
extern GLuint * tex;

void setPresence(AbstractPresence * p);

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
	DirectUniform(GLuint program, const GLchar *name);
	void set(AbstractPresence * b);
};

class UniformMatrix
{
private:
	GLint location;
	float * matrix;
public:
	UniformMatrix(GLuint program, const GLchar *name, float * matrix);
	void set(AbstractPresence * b);
};

class UniformTexture
{
private:
	GLint location;
public:
	UniformTexture(GLuint program, const GLchar *name);
	void set(AbstractPresence * b);
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
	VertexAttrib(GLuint program, GLuint index, const GLchar *name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
	~VertexAttrib();
	void set(AbstractPresence * b);
};

class StencilOperation
{
public:
	StencilOperation();
	void set(AbstractPresence * b);
};

class BackgroundRenderer
{
private:
	GLuint program;
public:
	BackgroundRenderer();
	~BackgroundRenderer();
	void process();
};

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

