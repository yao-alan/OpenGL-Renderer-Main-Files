#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "Callbacks.h"
#include "MonitorInfo.h"
#include "Movement.h"

void initCallbacks(GLFWwindow* window, int callback1, int callback2, int callback3) 
{
	if (callback1 == KEY_CALLBACK || callback2 == KEY_CALLBACK || callback3 == KEY_CALLBACK)
		glfwSetKeyCallback(window, keyInputCallback);
	if (callback1 == MOUSE_CALLBACK || callback2 == MOUSE_CALLBACK || callback3 == MOUSE_CALLBACK)
		glfwSetCursorPosCallback(window, mouseCallback);
	if (callback1 == FRAMEBUFFER_CALLBACK || callback2 == FRAMEBUFFER_CALLBACK || callback3 == FRAMEBUFFER_CALLBACK)
		glfwSetFramebufferSizeCallback(window, framebufferCallback);
}

void processKeyInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		moveInPlane(NORTH);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		moveInPlane(SOUTH);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		moveInPlane(WEST);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		moveInPlane(EAST);
	}
}

void keyInputCallback(GLFWwindow* window, int key, int keyCode, int action, int modifierBits) {
	;
}

void mouseCallback(GLFWwindow* window, double currentMouseX, double currentMouseY) 
{
	if (firstMouse) {
		firstMouse = false;
		lastMouseX = currentMouseX;
		lastMouseY = currentMouseY;
	} else {
		cameraRotation(currentMouseX, currentMouseY, 1);
	}
}

void framebufferCallback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

