extern GLuint vao;
extern GLuint * vbo;
extern GLsizei * vbSize;
extern GLuint * tex;

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

