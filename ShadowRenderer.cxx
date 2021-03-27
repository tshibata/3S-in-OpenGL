#include <stdlib.h>
#include <stdio.h>
#include <platform.h>
#include "ref.h"
#include "geometry.h"
#include "common.h"
#include "renderer.h"

ShadowRenderer::ShadowRenderer(Texture & shadowMap) : shadowMap(& shadowMap)
{
	const GLchar * vert =
		R"(#version 330 core
		uniform mat4 lmat;
		in vec3 xyz0;
		void main() {
			gl_Position = lmat * vec4(xyz0, 1.0);
		})";

	const GLchar * frag =
		R"(#version 330 core
		void main() {
			gl_FragDepth = gl_FragCoord.z;
		})";

	program = initProgram(& vert, & frag);
}
ShadowRenderer::~ShadowRenderer()
{
	if (program != 0)
	{
		glDeleteProgram(program);
	}
}
void ShadowRenderer::process()
{
	GLint target;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, & target);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->frameBuffer);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, shadowMap->width, shadowMap->height);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glBindVertexArray(vao);

	VertexAttrib<SpacialFigure> xyz0(program, 0, "xyz0", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (2 * sizeof(float)));
	UniformMatrix<SpacialFigure> lmat(program, "lmat", lightingMatrix);

	for (AbstractPresence<SpacialFigure> * b = AbstractPresence<SpacialFigure>::getFirst(); b != nullptr; b = b->getNext())
	{
	 	if (b->getFigure()->mode == SOLID_BLEND)
		{
			lmat.set(b);
			xyz0.set(b);

			glDrawArrays(GL_TRIANGLES, 0, vbSize[b->getFigure()->id] / (8 * sizeof(float)));
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, target);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

