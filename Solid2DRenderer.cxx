#include <utility>
#include <stdlib.h>
#include <stdio.h>
#include <platform.h>
#include "sss/Basis.h"
#include "sss/geometry.h"
#include "sss/common.h"
#include "sss/renderer.h"
#include "Solid2DRenderer.h"

RenderingMode solid2D;

Solid2DRenderer::Solid2DRenderer()
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
			vec4 color = texture(tex, uv1);
			if (color.a == 0)
			{
				discard;
			}
			gl_FragColor = color;
		})";

	program = initProgram(& vert, & frag);
}
Solid2DRenderer::~Solid2DRenderer()
{
	if (program != 0)
	{
		glDeleteProgram(program);
	}
}
void Solid2DRenderer::process()
{
	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBindVertexArray(vao);

	present(solid2D,
		StencilOperation(),
		DirectUniform(program, "fmat"),
		UniformTexture(program, "tex"),
		VertexAttrib(program, 0, "xyz0", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (2 * sizeof(float))),
		VertexAttrib(program, 1, "uv0", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0));
}

