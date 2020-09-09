#pragma once

#include <kazmath/kazmath.h>

// default coord system is right-handed
// (By default OpenGL's camera is looking down toward the -z side of the z-axis)
//
//            y                  
//            | -z
//            | /
//            |/
//  -x ------- ------- x
//           /|
//          / |
//         z  -y


typedef struct Camera 
{
	kmVec3 sceneOrigin;
	kmVec3 cameraPos;
	kmVec3 camera_xAxis;
	kmVec3 camera_yAxis;
	kmVec3 camera_zAxis;
	kmMat4 worldMatrix;
	kmMat4 viewMatrix;
	kmMat4 lookAt;
	kmMat4 perspectiveMatrix;
	kmMat4 orthographicMatrix;
	kmMat4 cameraToWorldspaceMatrix;
} Camera;

extern Camera camera;

void genDefaultCameraMatrices(Camera* camera);

//move objects into correct world-space coordinates and orientation
void genWorldMatrix(Camera* camera, kmVec3 worldPos, kmVec3 axis, double theta, kmVec3 scaleFactor);

//generate the matrix that determines how the camera sees world-space objects
void genViewMatrix(Camera* camera);

//generate perspective matrix
void genPerspectiveMatrix(Camera* camera, kmScalar fovY, kmScalar distNear, kmScalar distFar);

//generate orthographic matrix
void genOrthographicMatrix(Camera* camera);

//converts a vector written in camera-space and converts it to a vector in world-space
void genCameraToWorldspaceMatrix(Camera* camera);

//generate camera-space axes
void genCameraCoordSystem(Camera* camera);

//set where the camera is looking
void setSceneOrigin(Camera* camera, kmVec3 pos);

//set where the camera is relative to world-space
void setCameraPos(Camera* camera, kmVec3 pos);

void initCamera(Camera* camera, kmVec3 cameraPos, kmVec3 sceneOrigin);
