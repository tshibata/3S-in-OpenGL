#include <utility>
#include <stdlib.h>
#include <platform.h>
#include "sss/Basis.h"
#include "sss/geometry.h"
#include "sss/common.h"
#include "sss/renderer.h"
#include "LucidRenderer.h"

sss::RenderingMode lucid3D;

LucidRenderer::LucidRenderer()
{
	const GLchar * vert =
		R"(#version 330 core
		uniform mat4 fmat;
		layout (location = 0) in vec3 xyz0;
		layout (location = 1) in vec3 norm;
		layout (location = 2) in vec2 uv0;
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

	program = sss::initProgram(& vert, & frag);
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
	glBindVertexArray(sss::vao);

	present(lucid3D,
		sss::StencilOperation(),
		sss::VertexAttrib(program, 0, "xyz0", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (2 * sizeof(float))),
		sss::VertexAttrib(program, 1, "norm", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float))),
		sss::VertexAttrib(program, 2, "uv0", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0),
		sss::UniformMatrix(program, "fmat", framingMatrix),
		sss::UniformTexture(program, "tex"));
}

