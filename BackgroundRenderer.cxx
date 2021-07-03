#include <utility>
#include <stdlib.h>
#include <platform.h>
#include "sss/Basis.h"
#include "sss/geometry.h"
#include "sss/common.h"
#include "sss/renderer.h"
#include "BackgroundRenderer.h"

sss::RenderingMode background;

BackgroundRenderer::BackgroundRenderer()
{
	const GLchar * vert =
		R"(#version 330 core
		uniform mat4 fmat;
		layout (location = 0) in vec3 xyz0;
		layout (location = 1) in vec2 uv0;
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

	program = sss::initProgram(& vert, & frag);
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
	glBindVertexArray(sss::vao);

	present(background,
		sss::DirectUniform(program, "fmat"),
		sss::UniformTexture(program, "tex"),
		sss::VertexAttrib(program, 0, "xyz0", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (2 * sizeof(float))),
		sss::VertexAttrib(program, 1, "uv0", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0));
}

