#include <utility>
#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <platform.h>
#include "Basis.h"
#include "geometry.h"
#include "common.h"
#include "files.h"

namespace sss
{

static GLboolean initShader(GLuint shader, const GLchar * * source)
{
	int status;
	glShaderSource(shader, 1, source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, & status);
	if (status == GL_FALSE)
	{
		int len;
		char * buf;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, & len);
		buf = (char *) malloc(len + 1);
		glGetShaderInfoLog(shader, len, NULL, buf);
		std::fprintf(stderr, "Compile failed: %s\n", buf);
		free(buf);
		return GL_FALSE;
	}
	return GL_TRUE;
}

GLuint initProgram(const GLchar * * vertSource, const GLchar * * fragSource)
{
	GLuint program;
	program = 0;

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	if (initShader(vertShader, vertSource) && initShader(fragShader, fragSource))
	{
		GLint status;

		program = glCreateProgram();

		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);

		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, & status);
		if (status == GL_FALSE)
		{
			int len;
			char * buf;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, & len);
			buf = (char *) malloc(len + 1);
			glGetProgramInfoLog(program, len, NULL, buf);
			std::fprintf(stderr, "Link failed: %s\n", buf);
		}

		glDetachShader(program, vertShader);
		glDetachShader(program, fragShader);

		if (status == GL_FALSE)
		{
			glDeleteProgram(program);
			program = 0;
		}
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}

GLuint vao;
GLuint * vbo;
GLsizei * vbSize;
GLuint * tex;

static int frame = 0;
static struct timespec t0;

bool initiate()
{
	GLboolean result;

	tex = (GLuint *) malloc((Texture::nextId()) * sizeof(GLuint));
	glGenTextures(Texture::nextId(), tex);

	for (Texture * t = Texture::last; t != nullptr; t = t->next)
	{
		glGenFramebuffers(1, & t->frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, t->frameBuffer);
		glActiveTexture(GL_TEXTURE0 + t->id); // is this legal?
		glBindTexture(GL_TEXTURE_2D, tex[t->id]);
		if (t->path == nullptr)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, t->width, t->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			unsigned char * data;
			unsigned int type;
			readPng(t->path, & data, & t->width, & t->height, & type);
			glTexImage2D(GL_TEXTURE_2D, 0, type, t->width, t->height, 0, type, GL_UNSIGNED_BYTE, data);
			free(data);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex[t->id], 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glGenVertexArrays(1, & vao);

	vbo = (GLuint *) malloc((Figure::last->id + 1) * sizeof(GLuint));
	vbSize = (GLsizei *) malloc((Figure::last->id + 1) * sizeof(GLsizei));
	glGenBuffers(Figure::last->id + 1, vbo);
	for (Figure * f = Figure::last; f != nullptr; f = f->next)
	{
		unsigned char * data;
		size_t size;
		f->getData(& data, & size);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[f->id]);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		vbSize[f->id] = size;
		free(data);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // TBD: better use default?

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // not needed

	glEnable(GL_STENCIL_TEST);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearStencil(0);

	return true;
}

void terminate()
{
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, & vao);
	free(tex);
	free(vbo);
	free(vbSize);
}

bool update()
{
	static Scene * curr = nullptr;
	if (curr == nullptr)
	{
		clock_gettime(CLOCK_MONOTONIC, & t0);
		curr = arrange();
		if (curr == nullptr)
		{
			return false;
		}
	}
	else
	{
		frame++;
		struct timespec t1;
		clock_gettime(CLOCK_MONOTONIC, & t1);
		unsigned int dt = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_nsec / 1000 - t0.tv_nsec / 1000;
		if (t0.tv_sec < t1.tv_sec)
		{
			std::printf("%d frames/s\n", frame);
			frame = 0;
		}
		t0 = t1;
		Scene * next = curr->rearrange(dt);
		if (next == nullptr)
		{
			return false;
		}
		delete curr;
		curr = next;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	curr->render();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glFlush();

	return true;
}

unsigned char pixelLabel(float x, float y)
{
	unsigned char c;
	glReadPixels(x, screenHeight - y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, & c);
	return c;
}


Scene::Scene()
{
}
Scene::~Scene()
{
}

}
