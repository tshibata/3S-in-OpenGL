#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <platform.h>
#include "../common.h"

void errorHandler(int code, const char * description)
{
    fprintf(stderr, "Error %d: %s\n", code, description);
}

int main(int argc, char * * argv)
{
	if (glfwInit() == GL_FALSE) {
		return 1;
	}
	glfwSetErrorCallback(errorHandler);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow * const window(glfwCreateWindow(512, 512, "Shadow, Solid & Screen", NULL, NULL));
	if (window == NULL){
		return 2;
	}
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if(err != GLEW_OK){
		return 3;
	}

	if (! initiate())
	{
		return 4;
	}
	while (glfwWindowShouldClose(window) == GL_FALSE) {
		if (! update())
		{
			break;
		}
		glfwSwapBuffers(window);
		glfwWaitEventsTimeout(0.0);
	}
	terminate();
	return 0;
}
