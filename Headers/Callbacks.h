#pragma once

#include <stdbool.h>

#define KEY_CALLBACK 299
#define MOUSE_CALLBACK 300
#define FRAMEBUFFER_CALLBACK 301

extern bool firstMouse;

void initCallbacks(GLFWwindow* window, int callback1, int callback2, int callback3);

void processKeyInput(GLFWwindow* window);

void keyInputCallback(GLFWwindow* window, int key, int keyCode, int action, int modifierBits);

void mouseCallback(GLFWwindow* window, double x, double y);

void framebufferCallback(GLFWwindow* window, int width, int height);