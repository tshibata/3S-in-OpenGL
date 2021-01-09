#include <stdio.h>
#include <time.h>
#include <png.h>
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>
#include <platform.h>
#include "geometry.h"
#include "common.h"

const GLchar * SHADOW_VERT =
R"(#version 330 core
uniform mat4 lmat;
in vec3 xyz0;
void main() {
	gl_Position = lmat * vec4(xyz0, 1.0);
})";

const GLchar * SHADOW_FRAG =
R"(#version 330 core
void main() {
	gl_FragDepth = gl_FragCoord.z;
})";

const GLchar * SOLID_VERT =
R"(#version 330 core
uniform mat4 fmat;
uniform mat4 lmat;
in vec3 xyz0;
in vec3 norm;
in vec2 uv0;
out vec2 uv1;
out vec4 lxyz;
out vec4 lnorm;
void main() {
	gl_Position = fmat * vec4(xyz0, 1.0);
	uv1 = uv0;
	lxyz = lmat * vec4(xyz0, 1.0);
	lnorm = lmat * vec4(norm + xyz0, 1.0) - lxyz;
})";

const GLchar * SOLID_FRAG =
R"(#version 330 core
uniform sampler2D tex0;
uniform sampler2D tex1;
in vec2 uv1;
in vec4 lxyz;
in vec4 lnorm;
void main() {
	// illuminance, light per area
	float i = clamp(dot(vec4(0.0, 0.0, 1.0, 1.0), normalize(lnorm)), 0, 1);

	// coordinate in the shadow map
	float x = (lxyz.x / lxyz.w + 1.0) / 2;
	float y = (lxyz.y / lxyz.w + 1.0) / 2;
	float z = (lxyz.z / lxyz.w + 1.0) / 2;

	if (i != 0 && texture(tex0, vec2(x, y)).x < z - 0.01 / i)
	{
		i = 0;
	}
	gl_FragColor = (texture(tex1, uv1).rgba * (i + 0.5) / 1.5);
})";

const GLchar * SCREEN_VERT =
R"(#version 330 core
uniform mat4 fmat;
uniform mat4 lmat;
in vec3 xyz0;
in vec3 norm;
in vec2 uv0;
out vec2 uv1;
out vec4 fnorm;
void main() {
	gl_Position = fmat * vec4(xyz0, 1.0);
	uv1 = uv0;
	fnorm = fmat * vec4(norm + xyz0, 1.0) - gl_Position;
})";

const GLchar * SCREEN_FRAG =
R"(#version 330 core
uniform sampler2D tex1;
in vec2 uv1;
in vec4 fnorm;
void main() {
	float a = abs(dot(vec4(0.0, 0.0, 1.0, 1.0), normalize(fnorm)));
	a = 1.0 / (a + 1.0);
	gl_FragColor = vec4(texture(tex1, uv1).rgb * a, 1.0);
})";

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
		fprintf(stderr, "Compile failed: %s\n", buf);
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
			fprintf(stderr, "Link failed: %s\n", buf);
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

void readPNG(const char * path)
{
	png_byte sig[8];
	png_structp png;
	png_infop info;
	FILE * fp = fopen(path, "rb");
	if (! fp)
	{
		exit(1); // failed to open
	}
	if (fread(sig, sizeof(png_byte), sizeof(sig), fp) != sizeof(sig))
	{
		exit(2); // no signature
	}
	if (png_sig_cmp(sig, 0, sizeof(sig)) != 0)
	{
		exit(3); // wrong signature
	}
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (png == nullptr) {
		exit(4); // failed because of memory or whatever
	}
	info = png_create_info_struct(png);
	if (info == nullptr) {
		exit(5); // failed because of memory or whatever
	}
	png_init_io(png, fp);
	png_set_sig_bytes(png, sizeof(sig));
	int xform = PNG_TRANSFORM_PACKING | PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_EXPAND;
	png_read_png(png, info, xform, nullptr);
	int width = png_get_image_width(png, info);
	int height = png_get_image_height(png, info);
	png_bytepp rows = png_get_rows(png, info);
	int rowbytes = png_get_rowbytes(png, info);
	std::vector<unsigned char> buf;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < rowbytes; j++)
		{
			buf.push_back(rows[i][j]);
		}
	}
	switch (png_get_color_type(png, info))
	{
	case PNG_COLOR_TYPE_RGB:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buf.data());
	break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
	break;
	default:
		exit(6); // unsupported format
	}
	png_destroy_read_struct(& png, & info, nullptr);
	fclose(fp);
}

void readTxt(std::string name, GLuint vboIndex)
{
	static std::regex empty("^\\W*$");
	static std::regex point("^\\W*u:(.*)\\W+v:(.*)\\W+x:(.*)\\W+y:(.*)\\W+z:(.*)\\W+a:(.*)\\W+b:(.*)\\W+c:(.*)\\W*$");

	std::vector<float> vec;

	std::fstream f;
	f.open(name, std::ios_base::in);

	std::string line;
	std::smatch match;
	while (std::getline(f, line, '\n'))
	{
		if (regex_match(line, match, empty))
		{
			// nothing to do
		}
		else if (regex_match(line, match, point))
		{
			vec.push_back(std::stof(match[1]));
			vec.push_back(std::stof(match[2]));

			vec.push_back(std::stof(match[3]));
			vec.push_back(std::stof(match[4]));
			vec.push_back(std::stof(match[5]));

			vec.push_back(std::stof(match[6]));
			vec.push_back(std::stof(match[7]));
			vec.push_back(std::stof(match[8]));
		}
		else
		{
			exit(1);
		}
	}

	f.close();

	glBindBuffer(GL_ARRAY_BUFFER, vboIndex);
	glBufferData(GL_ARRAY_BUFFER, vec.size() * sizeof(float), vec.data(), GL_STATIC_DRAW);
}

static GLuint vao;
static GLuint * vbo;
static GLuint * tex;
static GLuint shadowProgram;
static GLuint solidProgram;
static GLuint screenProgram;
static GLuint shadowBuffer;
static int frame = 0;
static struct timespec t0;

bool initiate()
{
	GLboolean result;

	glGenVertexArrays(1, & vao);

	vbo = (GLuint *) malloc((Figure::last->id + 1) * sizeof(GLuint));
	glGenBuffers(Figure::last->id + 1, vbo);
	for (Figure * f= Figure::last; f != nullptr; f = f->next)
	{
		readTxt(f->path, vbo[f->id]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	tex = (GLuint *) malloc((Texture::last->id + 1) * sizeof(GLuint));
	glGenTextures(Texture::last->id + 1, tex);

	glGenFramebuffers(1, & shadowBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 512, 512, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex[0], 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	for (Texture * t = Texture::last; t != nullptr; t = t->next)
	{
		glActiveTexture(GL_TEXTURE0 + t->id); // is this legal?
		glBindTexture(GL_TEXTURE_2D, tex[t->id]);
		readPNG(t->path);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}


	shadowProgram = initProgram(& SHADOW_VERT, & SHADOW_FRAG);
	solidProgram = initProgram(& SOLID_VERT, & SOLID_FRAG);
	screenProgram = initProgram(& SCREEN_VERT, & SCREEN_FRAG);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // TBD: better use default?

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // not needed

	glClearColor(0.0, 0.0, 0.0, 1.0);

	return shadowProgram != 0 && solidProgram != 0 && screenProgram != 0;
}

void terminate()
{
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, & vao);
	if (shadowProgram != 0)
	{
		glDeleteProgram(shadowProgram);
	}
	if (solidProgram != 0)
	{
		glDeleteProgram(solidProgram);
	}
	if (screenProgram != 0)
	{
		glDeleteProgram(screenProgram);
	}
}

bool update(float x, float y)
{
	static Scene * curr = nullptr;
	if (curr == nullptr)
	{
		clock_gettime(CLOCK_MONOTONIC, & t0);
		curr = arrange(x, y);
	}
	else
	{
		frame++;
		struct timespec t1;
		clock_gettime(CLOCK_MONOTONIC, & t1);
		unsigned int dt = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_nsec / 1000 - t0.tv_nsec / 1000;
		if (t0.tv_sec < t1.tv_sec)
		{
			printf("%d frames/s\n", frame);
			frame = 0;
		}
		t0 = t1;
		Scene * next = curr->rearrange(dt, x, y);
		if (next != curr)
		{
			delete curr;
		}
		if (next == nullptr)
		{
			return false;
		}
		curr = next;
	}

	Matrix4x4 lightingMatrix;
	curr->lighting->getMatrix(lightingMatrix.elements);
	Matrix4x4 framingMatrix;
	curr->framing->getMatrix(framingMatrix.elements);

	GLint target;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, & target);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);

	// shadow

	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowProgram);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);

	for (Being * b = Being::getFirst(); b != nullptr; b = b->getNext()) if (b->getType() == SOLID_BEING)
	{
		GLfloat objectLightingMatrix[16];
		Matrix4x4 objectMatrix;
		b->getMatrix(objectMatrix.elements);

		prod(objectMatrix.elements, lightingMatrix.elements, objectLightingMatrix);

		glBindAttribLocation(shadowProgram, 0, "xyz0");

		glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "lmat"), 1, GL_FALSE, objectLightingMatrix);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[b->getFigure()->id]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (2 * sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, 3 * 5 * 2);
	}
	glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "framebuffer incomplete\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, target);
	glDisableVertexAttribArray(0);


	// solid

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(solidProgram);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (Being * b = Being::getFirst(); b != nullptr; b = b->getNext()) if (b->getType() == SOLID_BEING)
	{
		GLfloat objectLightingMatrix[16];
		GLfloat objectFramingMatrix[16];
		Matrix4x4 objectMatrix;
		b->getMatrix(objectMatrix.elements);

		prod(objectMatrix.elements, lightingMatrix.elements, objectLightingMatrix);
		prod(objectMatrix.elements, framingMatrix.elements, objectFramingMatrix);

		glUniformMatrix4fv(glGetUniformLocation(solidProgram, "fmat"), 1, GL_FALSE, objectFramingMatrix);
		glUniformMatrix4fv(glGetUniformLocation(solidProgram, "lmat"), 1, GL_FALSE, objectLightingMatrix);

		glUniform1i(glGetUniformLocation(solidProgram, "tex0"), 0);
		glUniform1i(glGetUniformLocation(solidProgram, "tex1"), b->getTexture()->id);

		glBindAttribLocation(solidProgram, 0, "xyz0");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[b->getFigure()->id]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (2 * sizeof(float)));

		glBindAttribLocation(solidProgram, 1, "norm");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[b->getFigure()->id]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));

		glBindAttribLocation(solidProgram, 2, "uv0");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[b->getFigure()->id]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

		glDrawArrays(GL_TRIANGLES, 0, 3 * 5 * 2);
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);


	// screen

	glUseProgram(screenProgram);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (Being * b = Being::getFirst(); b != nullptr; b = b->getNext()) if (b->getType() == SCREEN_BEING)
	{
		GLfloat objectLightingMatrix[16];
		GLfloat objectFramingMatrix[16];
		Matrix4x4 objectMatrix;
		b->getMatrix(objectMatrix.elements);

		prod(objectMatrix.elements, lightingMatrix.elements, objectLightingMatrix);
		prod(objectMatrix.elements, framingMatrix.elements, objectFramingMatrix);

		glUniformMatrix4fv(glGetUniformLocation(screenProgram, "fmat"), 1, GL_FALSE, objectFramingMatrix);
		glUniformMatrix4fv(glGetUniformLocation(screenProgram, "lmat"), 1, GL_FALSE, objectLightingMatrix);

		glUniform1i(glGetUniformLocation(screenProgram, "tex1"), b->getTexture()->id);

		glBindAttribLocation(screenProgram, 0, "xyz0");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[b->getFigure()->id]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (2 * sizeof(float)));

		glBindAttribLocation(screenProgram, 1, "norm");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[b->getFigure()->id]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));

		glBindAttribLocation(screenProgram, 2, "uv0");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[b->getFigure()->id]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

		glDrawArrays(GL_TRIANGLES, 0, 3 * 5 * 2);
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glFlush();

	return true;
}
