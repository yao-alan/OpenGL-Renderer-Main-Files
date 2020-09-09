#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <kazmath/kazmath.h>

#include "Camera.h"
#include "MonitorInfo.h"

void genDefaultCameraMatrices(Camera* camera) 
{
	kmVec3 worldPos;
	kmVec3Fill(&worldPos, 0, 0, 0);
	kmVec3 axis;
	kmVec3Fill(&axis, 1, 0, 0);
	kmVec3 scaleFactor;
	kmVec3Fill(&scaleFactor, 1, 1, 1);

	genWorldMatrix(camera, worldPos, axis, 45.0, scaleFactor);
	genViewMatrix(camera);
	genPerspectiveMatrix(camera, 90.0, 0.1, 100);
	genCameraToWorldspaceMatrix(camera);
}

//move objects into correct world-space orientation and coordinates
void genWorldMatrix(Camera* camera, kmVec3 worldPos, kmVec3 axis, double theta, kmVec3 scaleFactor)
{
	kmMat4 rotationMatrix;
	kmMat4RotationAxisAngle(&rotationMatrix, &axis, (float)theta);

	kmMat4 translationMatrix;
	kmMat4Translation(&translationMatrix, worldPos.x, worldPos.y, worldPos.z);

	kmMat4 scaleMatrix;
	kmMat4Scaling(&scaleMatrix, scaleFactor.x, scaleFactor.y, scaleFactor.z);

	kmMat4Multiply(&(camera->worldMatrix), &translationMatrix, &scaleMatrix);
	kmMat4Multiply(&(camera->worldMatrix), &rotationMatrix, &(camera->worldMatrix));
}

//matrix for camera POV
void genViewMatrix(Camera* camera) 
{
	genCameraCoordSystem(camera);

	kmMat4 translationMatrix;
	kmMat4Translation(&translationMatrix, -(camera->cameraPos.x), -(camera->cameraPos.y), -(camera->cameraPos.z));

	/* creating this matrix: */
	/* | i_x  i_y  i_z  0 | */
	/* | j_x  j_y  j_z  0 | */
	/* | k_x  k_y  k_z  0 | */
	/* |  0    0    0   1 | */

	kmMat4Identity(&(camera->lookAt));
	camera->lookAt.mat[0] = camera->camera_xAxis.x;
	camera->lookAt.mat[1] = camera->camera_yAxis.x;
	camera->lookAt.mat[2] = camera->camera_zAxis.x;
	camera->lookAt.mat[4] = camera->camera_xAxis.y;
	camera->lookAt.mat[5] = camera->camera_yAxis.y;
	camera->lookAt.mat[6] = camera->camera_zAxis.y;
	camera->lookAt.mat[8] = camera->camera_xAxis.z;
	camera->lookAt.mat[9] = camera->camera_yAxis.z;
	camera->lookAt.mat[10] = camera->camera_zAxis.z;

	kmMat4Multiply(&(camera->viewMatrix), &(camera->lookAt), &translationMatrix);
}

void genPerspectiveMatrix(Camera* camera, kmScalar fovY, kmScalar distNear, kmScalar distFar) 
{
	kmMat4PerspectiveProjection(&(camera->perspectiveMatrix), fovY, (float)(getHorizontalRes(0) / getVerticalRes(0)), distNear, distFar);
}

void genOrthographicMatrix(Camera* camera) 
{
	;
}

void genCameraToWorldspaceMatrix(Camera* camera) 
{
	kmMat4Inverse(&(camera->cameraToWorldspaceMatrix), &(camera->lookAt));
}

/* generating basis vectors via cross products */
void genCameraCoordSystem(Camera* camera) 
{
	/* generating camera's z-axis */
	kmVec3Fill(&(camera->camera_zAxis), (camera->cameraPos.x - camera->sceneOrigin.x), (camera->cameraPos.y - camera->sceneOrigin.y), (camera->cameraPos.z - camera->sceneOrigin.z));
	kmVec3Normalize(&(camera->camera_zAxis), &(camera->camera_zAxis));

	/* <0, 1, 0> for use in cross product to find x-axis */
	kmVec3 worldUpVector;
	kmVec3Fill(&worldUpVector, 0, 1, 0);

	/* generating camera's x-axis */
	kmVec3Cross(&(camera->camera_xAxis), &worldUpVector, &(camera->camera_zAxis));
	kmVec3Normalize(&(camera->camera_xAxis), &(camera->camera_xAxis));

	/* generating camera's y-axis via cross product with z-axis and x-axis */
	kmVec3Cross(&(camera->camera_yAxis), &(camera->camera_zAxis), &(camera->camera_xAxis));
	kmVec3Normalize(&(camera->camera_yAxis), &(camera->camera_yAxis));
}

/* where the camera is looking */
void setSceneOrigin(Camera* camera, kmVec3 pos) 
{
	camera->sceneOrigin = pos;
}

/* camera coordinates relative to worldspace origin */
void setCameraPos(Camera* camera, kmVec3 pos) 
{
	camera->cameraPos = pos;
}

void initCamera(Camera* camera, kmVec3 cameraPos, kmVec3 sceneOrigin) 
{
	setCameraPos(camera, cameraPos);
	setSceneOrigin(camera, sceneOrigin);
}

