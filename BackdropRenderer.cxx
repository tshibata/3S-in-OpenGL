#include <utility>
#include <stdlib.h>
#include <platform.h>
#include "sss/Basis.h"
#include "sss/geometry.h"
#include "sss/common.h"
#include "sss/renderer.h"
#include "BackdropRenderer.h"

sss::RenderingMode backdrop;

BackdropRenderer::BackdropRenderer()
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
BackdropRenderer::~BackdropRenderer()
{
	if (program != 0)
	{
		glDeleteProgram(program);
	}
}
void BackdropRenderer::process()
{
	glDepthMask(GL_FALSE);
	glUseProgram(program);
	glDisable(GL_BLEND);
	glBindVertexArray(sss::vao);

	present(backdrop,
		sss::DirectUniform(program, "fmat"),
		sss::UniformTexture(program, "tex"),
		sss::VertexAttrib(program, 0, "xyz0", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (2 * sizeof(float))),
		sss::VertexAttrib(program, 1, "uv0", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0));
}

