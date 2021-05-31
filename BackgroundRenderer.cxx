#include <utility>
#include <stdlib.h>
#include <stdio.h>
#include <platform.h>
#include "Basis.h"
#include "geometry.h"
#include "common.h"
#include "renderer.h"

BackgroundRenderer::BackgroundRenderer()
{
	const GLchar * vert =
		R"(#version 330 core
		uniform mat4 fmat;
		in vec3 xyz0;
		in vec2 uv0;
		out vec2 uv1;
		void main() {
			gl_Position = fmat * vec4(xyz0, 1.0);
			uv1 = uv0;
		})";

		const GLchar * frag =
		R"(#version 330 core
		uniform sampler2D tex;
		in vec2 uv1;
		void main() {
			gl_FragColor = texture(tex, uv1);
		})";

	program = initProgram(& vert, & frag);
}
BackgroundRenderer::~BackgroundRenderer()
{
	if (program != 0)
	{
		glDeleteProgram(program);
	}
}
void BackgroundRenderer::process()
{
	glDepthMask(GL_FALSE);
	glUseProgram(program);
	glDisable(GL_BLEND);
	glBindVertexArray(vao);

	present(background,
		DirectUniform(program, "fmat"),
		UniformTexture(program, "tex"),
		VertexAttrib(program, 0, "xyz0", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (2 * sizeof(float))),
		VertexAttrib(program, 1, "uv0", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0));
}

