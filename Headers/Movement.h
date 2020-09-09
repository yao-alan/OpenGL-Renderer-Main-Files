#pragma once

#define MOVEMENT_SPEED 0.025
#define WEST 0
#define EAST 1
#define NORTH 2
#define SOUTH 3

extern int lastMouseX, lastMouseY;

void moveInPlane(int dir);

void cameraRotation(double currentMouseX, double currentMouseY, double sensitivity);
