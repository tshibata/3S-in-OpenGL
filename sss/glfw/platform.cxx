#include <cstdio>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <platform.h>
#include "../common.h"

sss::Controller sss::controllers[1];

void errorHandler(int code, const char * description)
{
	std::fprintf(stderr, "Error %d: %s\n", code, description);
}

void scroll(GLFWwindow * window, double dx, double dy)
{
	double x, y;
	glfwGetCursorPos(window, & x, & y);
	sss::controllers[0].x = x;
	sss::controllers[0].y = y;
	sss::wheelMoved(dy);
}

void click(GLFWwindow * window, int button, int action, int modifier)
{
	double x, y;
	glfwGetCursorPos(window, & x, & y);
	sss::controllers[0].x = x;
	sss::controllers[0].y = y;
	switch (button)
	{
		case GLFW_MOUSE_BUTTON_1:
		switch (action)
		{
			case GLFW_PRESS:
				sss::buttonPressed(-1);
				break;
			case GLFW_RELEASE:
				sss::buttonReleased(-1);
				break;
		}
		break;
		case GLFW_MOUSE_BUTTON_2:
		switch (action)
		{
			case GLFW_PRESS:
				sss::buttonPressed(1);
				break;
			case GLFW_RELEASE:
				sss::buttonReleased(1);
				break;
		}
		break;
		case GLFW_MOUSE_BUTTON_3:
		switch (action)
		{
			case GLFW_PRESS:
				sss::buttonPressed(0);
				break;
			case GLFW_RELEASE:
				sss::buttonReleased(0);
				break;
		}
		break;
	}
}

int main(int argc, char * * argv)
{
	struct timespec t0, t1;

	if (glfwInit() == GL_FALSE) {
		return 1;
	}
	glfwSetErrorCallback(errorHandler);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow * const window(glfwCreateWindow(sss::screenWidth, sss::screenHeight, sss::screenTitle, NULL, NULL));
	if (window == NULL){
		return 2;
	}
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if(err != GLEW_OK){
		return 3;
	}

	if (! sss::initiate())
	{
		return 4;
	}

	glfwSetScrollCallback(window, scroll);
	glfwSetMouseButtonCallback(window, click);

	clock_gettime(CLOCK_MONOTONIC, & t0);

	while (glfwWindowShouldClose(window) == GL_FALSE) {
		double x, y;
		glfwGetCursorPos(window, & x, & y);
		sss::controllers[0].x = x;
		sss::controllers[0].y = y;
		clock_gettime(CLOCK_MONOTONIC, & t1);
		unsigned int dt = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_nsec / 1000 - t0.tv_nsec / 1000;
		if (! sss::update(dt))
		{
			break;
		}
		glfwSwapBuffers(window);
		glfwWaitEventsTimeout(0.0);
		t0 = t1;
	}
	sss::terminate();
	return 0;
}
