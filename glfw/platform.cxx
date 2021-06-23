#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <platform.h>
#include "../common.h"

Controller controllers[1];

void errorHandler(int code, const char * description)
{
    fprintf(stderr, "Error %d: %s\n", code, description);
}

void scroll(GLFWwindow * window, double dx, double dy)
{
	double x, y;
	glfwGetCursorPos(window, & x, & y);
	controllers[0].x = x;
	controllers[0].y = y;
	wheelMoved(dy);
}

void click(GLFWwindow * window, int button, int action, int modifier)
{
	double x, y;
	glfwGetCursorPos(window, & x, & y);
	controllers[0].x = x;
	controllers[0].y = y;
	switch (button)
	{
		case GLFW_MOUSE_BUTTON_1:
		switch (action)
		{
			case GLFW_PRESS:
				buttonPressed(-1);
				break;
			case GLFW_RELEASE:
				buttonReleased(-1);
				break;
		}
		break;
		case GLFW_MOUSE_BUTTON_2:
		switch (action)
		{
			case GLFW_PRESS:
				buttonPressed(1);
				break;
			case GLFW_RELEASE:
				buttonReleased(1);
				break;
		}
		break;
		case GLFW_MOUSE_BUTTON_3:
		switch (action)
		{
			case GLFW_PRESS:
				buttonPressed(0);
				break;
			case GLFW_RELEASE:
				buttonReleased(0);
				break;
		}
		break;
	}
}

int main(int argc, char * * argv)
{
	if (glfwInit() == GL_FALSE) {
		return 1;
	}
	glfwSetErrorCallback(errorHandler);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow * const window(glfwCreateWindow(screenWidth, screenHeight, "Shadow, Solid & Screen", NULL, NULL));
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

	glfwSetScrollCallback(window, scroll);
	glfwSetMouseButtonCallback(window, click);

	while (glfwWindowShouldClose(window) == GL_FALSE) {
		double x, y;
		glfwGetCursorPos(window, & x, & y);
		controllers[0].x = x;
		controllers[0].y = y;
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
