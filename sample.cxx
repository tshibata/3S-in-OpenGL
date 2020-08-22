#include <stdio.h>
#include <math.h>
#include <time.h>
#include <platform.h>
#include "common.h"
#include "geometry.h"

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

static GLfloat earthABC[18];
static GLfloat earthXYZ[18] = {
	-9.9f, -1.0567f, -1.9f,
	-9.9f, -1.0567f, 9.9f,
	9.9f, -1.0567f, -1.9f,

	9.9f, -1.0567f, 9.9f,
	9.9f, -1.0567f, -1.9f,
	-9.9f, -1.0567f, 9.9f,
};
static GLfloat earthUV[] = {
  0.25f, 0.25f,
  0.25f, 1.0f,
  1.0f, 0.25f,

  1.0f, 1.0f,
  1.0f, 0.25f,
  0.25f, 1.0f,
};

static GLfloat starABC[90];
/*
sin 72 = 0.951
cos 72 = 0.309
sin 36 = 0.588
cos 36 = 0.809
*/
static GLfloat starXYZ[90] = {
  0.0f,  0.0f, 0.3f,
  0.0f,  1.0f, 0.0f,
  -0.588f, -0.809f, 0.0f,

  0.0f,  0.0f, 0.3f,
  -0.588f, -0.809f, 0.0f,
  0.951f,  0.309f, 0.0f,

  0.0f,  0.0f, 0.3f,
  0.951f,  0.309f, 0.0f,
  -0.951f,  0.309f, 0.0f,

  0.0f,  0.0f, 0.3f,
  -0.951f,  0.309f, 0.0f,
  0.588f, -0.809f, 0.0f,

  0.0f,  0.0f, 0.3f,
  0.588f, -0.809f, 0.0f,
  0.0f,  1.0f, 0.0f,


  0.0f,  0.0f, -0.3f,
  -0.588f, -0.809f, 0.0f,
  0.0f,  1.0f, 0.0f,

  0.0f,  0.0f, -0.3f,
  0.951f,  0.309f, 0.0f,
  -0.588f, -0.809f, 0.0f,

  0.0f,  0.0f, -0.3f,
  -0.951f,  0.309f, 0.0f,
  0.951f,  0.309f, 0.0f,

  0.0f,  0.0f, -0.3f,
  0.588f, -0.809f, 0.0f,
  -0.951f,  0.309f, 0.0f,

  0.0f,  0.0f, -0.3f,
  0.0f,  1.0f, 0.0f,
  0.588f, -0.809f, 0.0f,
};

static GLfloat starUV1[] = {
  10.0f, 10.0f,
  10.0f, 0.0f,
  0.0f, 10.0f,

  10.0f, 10.0f,
  10.0f, 0.0f,
  0.0f, 10.0f,

  10.0f, 10.0f,
  10.0f, 0.0f,
  0.0f, 10.0f,

  10.0f, 10.0f,
  10.0f, 0.0f,
  0.0f, 10.0f,

  10.0f, 10.0f,
  10.0f, 0.0f,
  0.0f, 10.0f,


  10.0f, 10.0f,
  10.0f, 0.0f,
  0.0f, 10.0f,

  10.0f, 10.0f,
  10.0f, 0.0f,
  0.0f, 10.0f,

  10.0f, 10.0f,
  10.0f, 0.0f,
  0.0f, 10.0f,

  10.0f, 10.0f,
  10.0f, 0.0f,
  0.0f, 10.0f,

  10.0f, 10.0f,
  10.0f, 0.0f,
  0.0f, 10.0f,
};

static GLfloat starUV2[] = {
  0.0f + 0.125f, 0.125f,
  0.0f + 0.125f, 0.125f,
  0.0f + 0.125f, 0.125f,

  0.25f + 0.125f, 0.125f,
  0.25f + 0.125f, 0.125f,
  0.25f + 0.125f, 0.125f,

  0.5f + 0.125f, 0.125f,
  0.5f + 0.125f, 0.125f,
  0.5f + 0.125f, 0.125f,

  0.75f + 0.125, 0.125f,
  0.75f + 0.125, 0.125f,
  0.75f + 0.125, 0.125f,

  0.0f + 0.125f, 0.25f + 0.125f,
  0.0f + 0.125f, 0.25f + 0.125f,
  0.0f + 0.125f, 0.25f + 0.125f,


  0.25f + 0.125f, 0.125f,
  0.25f + 0.125f, 0.125f,
  0.25f + 0.125f, 0.125f,

  0.5f + 0.125f, 0.125f,
  0.5f + 0.125f, 0.125f,
  0.5f + 0.125f, 0.125f,

  0.75f + 0.125, 0.125f,
  0.75f + 0.125, 0.125f,
  0.75f + 0.125, 0.125f,

  0.0f + 0.125f, 0.25f + 0.125f,
  0.0f + 0.125f, 0.25f + 0.125f,
  0.0f + 0.125f, 0.25f + 0.125f,

  0.0f + 0.125f, 0.125f,
  0.0f + 0.125f, 0.125f,
  0.0f + 0.125f, 0.125f,
};

static unsigned char textureData1[] = {
	0, 200, 0, 255,
	0, 200, 200, 255,
	0, 0, 200, 255,
	200, 0, 200, 255,

	200, 0, 0, 255,
	200, 200, 200, 255,
	200, 200, 200, 255,
	200, 200, 200, 255,

	0, 0, 0, 255,
	200, 200, 200, 255,
	250, 250, 250, 255,
	200, 200, 200, 255,

	0, 0, 0, 255,
	200, 200, 200, 255,
	200, 200, 200, 255,
	200, 200, 200, 255,
};

static unsigned char textureData2[] = {
	200, 200, 100, 255,
	200, 200, 100, 255,
	200, 200, 100, 255,
	200, 200, 100, 255,

	200, 200, 100, 255,
	250, 250, 200, 255,
	200, 200, 100, 255,
	200, 200, 100, 255,

	200, 200, 100, 255,
	200, 200, 100, 255,
	200, 200, 100, 255,
	200, 200, 100, 255,

	200, 200, 100, 255,
	200, 200, 100, 255,
	200, 200, 100, 255,
	200, 200, 100, 255,
};

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

static GLuint vao;
static GLuint vbo[7]; // xyz, abc, uv, uv, xyz, abc, uv
static GLuint tex[3]; // shadow and 2 surfaces
static GLuint shadowProgram;
static GLuint solidProgram;
static GLuint screenProgram;
static GLuint shadowBuffer;

static struct timespec t0;

GLboolean initiate()
{
	GLboolean result;

	for (int i = 0; i < 10; i ++)
	{
		norm(starXYZ, starABC, 9 * i);
	}
	for (int i = 0; i < 2; i ++)
	{
		norm(earthXYZ, earthABC, 9 * i);
	}

	glGenVertexArrays(1, & vao);

	glGenBuffers(7, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starXYZ), starXYZ, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starABC), starABC, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starUV1), starUV1, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starUV2), starUV2, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(earthXYZ), earthXYZ, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(earthABC), earthABC, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(earthUV), earthUV, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenTextures(3, tex);

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


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	shadowProgram = initProgram(& SHADOW_VERT, & SHADOW_FRAG);
	solidProgram = initProgram(& SOLID_VERT, & SOLID_FRAG);
	screenProgram = initProgram(& SCREEN_VERT, & SCREEN_FRAG);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // TBD: better use default?

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // not needed

	glClearColor(0.0, 0.0, 0.0, 1.0);

	clock_gettime(CLOCK_MONOTONIC, & t0);

	return shadowProgram != 0 && solidProgram != 0 && screenProgram != 0;
}

void terminate()
{
	glDeleteBuffers(7, vbo);
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

static int frame = 0;
static float angle1 = 0.0;
static float angle2 = 0.0;

GLboolean update()
{
	struct timespec t1;
	clock_gettime(CLOCK_MONOTONIC, & t1);
	if (t0.tv_sec < t1.tv_sec)
	{
		printf("%d frames/s\n", frame);
		frame = 0;
	}
	double dt = t1.tv_sec - t0.tv_sec + (t1.tv_nsec - t0.tv_nsec) * 0.000000001L;
	t0 = t1;
	frame++;

	GLint target;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, & target);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);

	GLfloat lightingMatrix[16];
	init(lightingMatrix);

	GLfloat framingMatrix[16];
	init(framingMatrix);

	GLfloat objectLightingMatrix[16];
	GLfloat objectFramingMatrix[16];

	rotY(lightingMatrix, 1.0f);
	rotX(lightingMatrix, 0.5f);
	move(lightingMatrix, 2.0f, 0.0f, 2.0f);
	proj(lightingMatrix, 1.0f, 1.0f, 3.0f);

	proj(framingMatrix, 1.0f, 1.0f, 3.0f);

	GLfloat objectMatrix[16];


	angle1 += 1.3 * dt / 1;
	angle2 += 0.2 * dt / 1;


	// shadow

	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowProgram);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);

	// draw the stars
	for (int i = 0; i < 2; i++)
	{
		init(objectMatrix);
		rotY(objectMatrix, i - angle1);
		move(objectMatrix, cosf(i - angle2) * 2.0, 0.0f, 3.0f + sinf(i - angle2) * 2.0);

		prod(objectMatrix, lightingMatrix, objectLightingMatrix);

		glBindAttribLocation(shadowProgram, 0, "xyz0");

		glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "lmat"), 1, GL_FALSE, objectLightingMatrix);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

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

	// draw the floor

	glUniformMatrix4fv(glGetUniformLocation(solidProgram, "fmat"), 1, GL_FALSE, framingMatrix);
	glUniformMatrix4fv(glGetUniformLocation(solidProgram, "lmat"), 1, GL_FALSE, lightingMatrix);

	glUniform1i(glGetUniformLocation(solidProgram, "tex0"), 0);
	glUniform1i(glGetUniformLocation(solidProgram, "tex1"), 1); // TBD better rename?

	glBindAttribLocation(solidProgram, 0, "xyz0");
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindAttribLocation(solidProgram, 1, "norm");
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindAttribLocation(solidProgram, 2, "uv0");
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

	// draw the stars
	for (int i = 0; i < 2; i++)
	{
		init(objectMatrix);
		rotY(objectMatrix, i - angle1);
		move(objectMatrix, cosf(i - angle2) * 2.0, 0.0f, 3.0f + sinf(i - angle2) * 2.0);

		prod(objectMatrix, lightingMatrix, objectLightingMatrix);
		prod(objectMatrix, framingMatrix, objectFramingMatrix);

		glUniformMatrix4fv(glGetUniformLocation(solidProgram, "fmat"), 1, GL_FALSE, objectFramingMatrix);
		glUniformMatrix4fv(glGetUniformLocation(solidProgram, "lmat"), 1, GL_FALSE, objectLightingMatrix);

		glUniform1i(glGetUniformLocation(solidProgram, "tex0"), 0);
		glUniform1i(glGetUniformLocation(solidProgram, "tex1"), 2);

		glBindAttribLocation(solidProgram, 0, "xyz0");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindAttribLocation(solidProgram, 1, "norm");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindAttribLocation(solidProgram, 2, "uv0");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

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

	// draw the stars
	for (int i = 2; i < 4; i++)
	{
		init(objectMatrix);
		rotY(objectMatrix, i - angle1);
		move(objectMatrix, cosf(i - angle2) * 2.0, 0.0f, 3.0f + sinf(i - angle2) * 2.0);

		prod(objectMatrix, lightingMatrix, objectLightingMatrix);
		prod(objectMatrix, framingMatrix, objectFramingMatrix);

		glUniformMatrix4fv(glGetUniformLocation(screenProgram, "fmat"), 1, GL_FALSE, objectFramingMatrix);
		glUniformMatrix4fv(glGetUniformLocation(screenProgram, "lmat"), 1, GL_FALSE, objectLightingMatrix);

		glUniform1i(glGetUniformLocation(screenProgram, "tex1"), 1);

		glBindAttribLocation(screenProgram, 0, "xyz0");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindAttribLocation(screenProgram, 1, "norm");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindAttribLocation(screenProgram, 2, "uv0");
		glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 3 * 5 * 2);
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glFlush();

	return GL_TRUE;
}

