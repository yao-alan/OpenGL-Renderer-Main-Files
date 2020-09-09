#define _USE_MATH_DEFINES

#include <kazmath/kazmath.h>
#include <math.h>

#include "Camera.h"
#include "FramerateTracker.h"
#include "Movement.h"

void moveInPlane(int dir) 
{
	kmVec3 move;
	float movementSpeed = MOVEMENT_SPEED * deltaTime * 100;

	if (dir == WEST) {
		kmVec3Fill(&move, -movementSpeed, 0, 0);
	}
	else if (dir == EAST) {
		kmVec3Fill(&move, movementSpeed, 0, 0);
	}
	else if (dir == NORTH) {
		kmVec3Fill(&move, 0, 0, -movementSpeed);
	}
	else if (dir == SOUTH) {
		kmVec3Fill(&move, 0, 0, movementSpeed);
	}

	kmVec3MultiplyMat4(&move, &move, &(camera.cameraToWorldspaceMatrix));
	kmVec3Add(&(camera.cameraPos), &(camera.cameraPos), &move);
	kmVec3Add(&(camera.sceneOrigin), &(camera.sceneOrigin), &move);
}

void cameraRotation(double currentMouseX, double currentMouseY, double sensitivity) 
{
	double dx, dy, z;
	dx = lastMouseX - currentMouseX;
	dy = currentMouseY - lastMouseY;
	z = 4000; //will adjust sensitivity by reducing the angle of turning when moving the mouse (increasing z decreases sensitivity)

	lastMouseX = currentMouseX;
	lastMouseY = currentMouseY;

	double theta1 = 0, theta2 = 0;
	if (dx >= 0 && dy >= 0)
		theta1 = atan(dy / dx);
	else if (dx <= 0 && dy >= 0)
		theta1 = M_PI + atan(dy / dx);
	else if (dx >= 0 && dy <= 0)
		theta1 = atan(dy / dx);
	else
		theta1 = M_PI + atan(dy / dx);
	theta2 = atan(sqrt(pow(dx, 2) + pow(dy, 2)) / z);

	//new camera x-axis
	kmMat4 rotationMatrix;
	kmMat4RotationAxisAngle(&rotationMatrix, &(camera.camera_zAxis), theta1);
	kmVec3MultiplyMat4(&(camera.camera_xAxis), &(camera.camera_xAxis), &rotationMatrix);

	//generate rotation axis (new camera y-axis)
	kmVec3Cross(&(camera.camera_yAxis), &(camera.camera_zAxis), &(camera.camera_xAxis));

	//vec = sceneOrigin - cameraPos; rotate vec by n(theta) where n is a sensitivity multiplier
	kmVec3 vec;
	kmVec3Subtract(&vec, &(camera.sceneOrigin), &(camera.cameraPos));
	kmMat4RotationAxisAngle(&rotationMatrix, &(camera.camera_yAxis), sensitivity * theta2);
	kmVec3MultiplyMat4(&vec, &vec, &rotationMatrix);

	//add vec and cameraPos to get new sceneOrigin
	kmVec3Add(&(camera.sceneOrigin), &(camera.cameraPos), &vec);

	genViewMatrix(&camera);
}