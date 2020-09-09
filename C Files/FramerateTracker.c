#define _CRT_SECURE_NO_DEPRECATE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include "FramerateTracker.h"

char* calculateFPS() 
{
	deltaTime = (float)glfwGetTime() - lastFrameTime;
	int fps = 1 / deltaTime;
	lastFrameTime += deltaTime;

	char* fpsString = (char*)calloc(5, sizeof(char));
	sprintf(fpsString, "%d", fps);

	free(fpsString);
	return fpsString;
}
