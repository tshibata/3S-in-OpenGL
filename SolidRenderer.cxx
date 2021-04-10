#include <stdlib.h>
#include <stdio.h>
#include <platform.h>
#include "ref.h"
#include "geometry.h"
#include "common.h"
#include "renderer.h"

SolidRenderer::SolidRenderer(Texture & shadowMap) : shadowMap(& shadowMap)
{
	const GLchar * vert =
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

	const GLchar * frag =
		R"(#version 330 core
		uniform sampler2D shadowMap;
		uniform sampler2D tex;
		in vec2 uv1;
		in vec4 lxyz;
		in vec4 lnorm;
		void main() {
			vec4 color = texture(tex, uv1);
			if (color.a == 0)
			{
				discard;
			}

			// illuminance, light per area
			float i = clamp(dot(vec4(0.0, 0.0, 1.0, 1.0), normalize(lnorm)), 0, 1);

			// coordinate in the shadow map
			float x = (lxyz.x / lxyz.w + 1.0) / 2;
			float y = (lxyz.y / lxyz.w + 1.0) / 2;
			float z = (lxyz.z / lxyz.w + 1.0) / 2;
			if (texture(shadowMap, vec2(x, y)).x < z - 0.01 / i)
			{
				i = 0;
			}

			gl_FragColor = (color * (i + 0.5) / 1.5);
		})";

	program = initProgram(& vert, & frag);
}
SolidRenderer::~SolidRenderer()
{
	if (program != 0)
	{
		glDeleteProgram(program);
	}
}
void SolidRenderer::process()
{
	glUseProgram(program);
	glBindVertexArray(vao);

	StencilOperation sop;
	VertexAttrib xyz0(program, 0, "xyz0", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (2 * sizeof(float)));
	VertexAttrib abc0(program, 1, "norm", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
	VertexAttrib uv0(program, 2, "uv0", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	UniformMatrix fmat(program, "fmat", framingMatrix);
	UniformMatrix lmat(program, "lmat", lightingMatrix);
	UniformTexture tex(program, "tex");

	glUniform1i(glGetUniformLocation(program, "shadowMap"), shadowMap->id);

	for (AbstractPresence * b = solid3D.getFirst(); b != nullptr; b = b->getNext())
	{
		sop.set(b);

		fmat.set(b);
		lmat.set(b);
		tex.set(b);
		xyz0.set(b);
		abc0.set(b);
		uv0.set(b);

		glDrawArrays(GL_TRIANGLES, 0, vbSize[b->getFigure()->id] / (8 * sizeof(float)));
	}
}

