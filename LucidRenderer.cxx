#include <utility>
#include <stdlib.h>
#include <stdio.h>
#include <platform.h>
#include "Basis.h"
#include "geometry.h"
#include "common.h"
#include "renderer.h"

LucidRenderer::LucidRenderer()
{
	const GLchar * vert =
		R"(#version 330 core
		uniform mat4 fmat;
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

	const GLchar * frag =
		R"(#version 330 core
		uniform sampler2D tex;
		in vec2 uv1;
		in vec4 fnorm;
		void main() {
			float a = abs(dot(vec4(0.0, 0.0, 1.0, 1.0), normalize(fnorm)));
			a = 1.0 / (a + 1.0);
			gl_FragColor = vec4(texture(tex, uv1).rgb * a, 1.0);
		})";

	program = initProgram(& vert, & frag);
}
LucidRenderer::~LucidRenderer()
{
	if (program != 0)
	{
		glDeleteProgram(program);
	}
}
void LucidRenderer::process()
{
	glUseProgram(program);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glBindVertexArray(vao);

	present(lucid3D,
		StencilOperation(),
		VertexAttrib(program, 0, "xyz0", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (2 * sizeof(float))),
		VertexAttrib(program, 1, "norm", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float))),
		VertexAttrib(program, 2, "uv0", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0),
		UniformMatrix(program, "fmat", framingMatrix),
		UniformTexture(program, "tex"));
}

