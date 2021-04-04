#include <stdlib.h>
#include <stdio.h>
#include <platform.h>
#include "ref.h"
#include "geometry.h"
#include "common.h"
#include "renderer.h"

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

	DirectUniform<SurficialFigure> mat(program, "fmat");
	UniformTexture<SurficialFigure> tex(program, "tex");
	VertexAttrib<SurficialFigure> xyz0(program, 0, "xyz0", 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
	VertexAttrib<SurficialFigure> uv0(program, 1, "uv0", 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	for (AbstractPresence<SurficialFigure> * b = AbstractPresence<SurficialFigure>::getFirst(); b != nullptr; b = b->getNext())
	{
		glStencilOp(GL_KEEP, GL_KEEP, b->label ? GL_REPLACE : GL_KEEP);
		glStencilFunc(GL_ALWAYS, b->label, 0xFF);

		mat.set(b);
		tex.set(b);
		xyz0.set(b);
		uv0.set(b);

		glDrawArrays(GL_TRIANGLES, 0, vbSize[b->getFigure()->id] / (5 * sizeof(float)));
	}
}

