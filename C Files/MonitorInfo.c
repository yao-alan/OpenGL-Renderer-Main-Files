#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "MonitorInfo.h"

int getHorizontalRes(int horizontal) 
{
	if (horizontal == DEFAULT_HORIZONTAL_RES) {
		return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
	}
	else {
		unsigned int viewportData[4];
		glGetIntegerv(GL_VIEWPORT, viewportData);
		return viewportData[2];
	}
}

int getVerticalRes(int vertical)
{
	if (vertical == DEFAULT_VERTICAL_RES) {
		return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
	}
	else {
		unsigned int viewportData[4];
		glGetIntegerv(GL_VIEWPORT, viewportData);
		return viewportData[3];
	}
}
