#define _CRT_SECURE_NO_WARNINGS

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "Callbacks.h"
#include "DataTypes.h"
#include "FramerateTracker.h"
#include "Mesh.h"
#include "Object.h"
#include "Renderer.h"
#include "Setup.h"

Camera camera;
bool firstMouse = true;
int lastMouseX = 0, lastMouseY = 0;
float deltaTime = 0.0, lastFrameTime = 0.0;

int main(void) 
{
	GLFWwindow* window = initContext();

	/* initialize callbacks */
	initCallbacks(window, KEY_CALLBACK, MOUSE_CALLBACK, FRAMEBUFFER_CALLBACK);

	/* generate shaders */
	unsigned int objShaderProgram = initShaders("vertexShaderSource.vshdr", "fragmentShaderSource.fshdr");
	unsigned int lightShaderProgram = initShaders("vertexShaderSource.vshdr", "lightSourceShader.fshdr");

	/* generate Object references */
	Object cube;
	Object directionalLight[2];
	Object pointLight[5];
	Object spotlight[5];

	/* set cube Material properties */
	kmVec3 vec;
	cube.material.ambient = *kmVec3Fill(&vec, 1.0f, 1.0f, 1.0f);
	cube.material.diffuse = *kmVec3Fill(&vec, 1.0f, 1.0f, 1.0f);
	cube.material.specular = *kmVec3Fill(&vec, 1.0f, 1.0f, 1.0f);
	cube.material.shininess = 8.0f;
	
	/* set light Material properties */
	pointLight[0].material.ambient = *kmVec3Fill(&vec, 0.1f, 0.1f, 0.1f);
	pointLight[0].material.diffuse = *kmVec3Fill(&vec, 0.8f, 0.8f, 0.8f);
	pointLight[0].material.specular = *kmVec3Fill(&vec, 1.0f, 1.0f, 1.0f);
	pointLight[0].attenuation.constant = 1.0f;
	pointLight[0].attenuation.linear = 0.09f;
	pointLight[0].attenuation.quadratic = 0.032f;

	/* set flashlight Material properties */
	spotlight[0].material.ambient = *kmVec3Fill(&vec, 0.1f, 0.1f, 0.1f);
	spotlight[0].material.diffuse = *kmVec3Fill(&vec, 1.0f, 1.0f, 1.0f);
	spotlight[0].material.specular = *kmVec3Fill(&vec, 1.0f, 1.0f, 1.0f);
	spotlight[0].attenuation.constant = 1.0f;
	spotlight[0].attenuation.linear = 0.09f;
	spotlight[0].attenuation.quadratic = 0.032f;
	spotlight[0].spotlight.lookDir = camera.camera_zAxis;
	spotlight[0].spotlight.innerCutoff = 0.95f;
	spotlight[0].spotlight.outerCutoff = 0.85f;

	/* set Object locations */
	kmVec3 cubePos;
	kmVec3Fill(&cubePos, 0, 0, 0);
	kmVec3 pointLightPos;
	kmVec3Fill(&pointLightPos, 0, 0, 3);
	kmVec3 spotLightPos;
	kmVec3Fill(&spotLightPos, camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);

	cube.worldSpace.pos = cubePos;
	pointLight[0].worldSpace.pos = pointLightPos;
	spotlight[0].worldSpace.pos = spotLightPos;

	/* generate buffers; declare array of Buffers if needed */
	Buffers cubeBuffers = *genBuffers();
	Buffers pointLightBuffer1 = *genBuffers();
	cube.buffer = cubeBuffers;
	pointLight[0].buffer = pointLightBuffer1;

	/* vertex data */
	float vertices[] = {
		/* positions */			  /* textures */   /* normal vectors */
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
	};

	writeToVBO(&(cube.buffer), vertices, sizeof(vertices), GL_STATIC_DRAW);
	/* how to interpret vertex data */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	writeToVBO(&(pointLight[0].buffer), vertices, sizeof(vertices), GL_STATIC_DRAW);
	/* how to interpret vertex data */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	/* adding diffuse maps (textures) and specular maps */
	initVector(&cube.material.diffuseMap, sizeof(unsigned int*));
	initVector(&cube.material.specularMap, sizeof(unsigned int*));
	initVector(&cube.material.emissiveMap, sizeof(unsigned int*));
	push_back(&cube.material.diffuseMap, (void*)useTexture("TextureLib/", GL_TEXTURE_2D));
	push_back(&cube.material.diffuseMap, (void*)useTexture("TextureLib/Container2_DiffuseMap.png", GL_TEXTURE_2D));
	push_back(&cube.material.specularMap, (void*)useTexture("TextureLib/Container2_SpecularMap.png", GL_TEXTURE_2D));
	push_back(&cube.material.emissiveMap, (void*)useTexture("TextureLib/", GL_TEXTURE_2D));

	/* assign textures to texture units */
	glUseProgram(objShaderProgram);
	glUniform1i(glGetUniformLocation(objShaderProgram, "material.diffuseMap[0]"), 0);
	glUniform1i(glGetUniformLocation(objShaderProgram, "material.diffuseMap[1]"), 1);
	glUniform1i(glGetUniformLocation(objShaderProgram, "material.specularMap"), 2);
	glUniform1i(glGetUniformLocation(objShaderProgram, "material.emissiveMap"), 3);

	
	/* setting material properties in shader */
	glUniform3f(glGetUniformLocation(objShaderProgram, "material.ambient"), cube.material.ambient.x, cube.material.ambient.y, cube.material.ambient.z);
	glUniform3f(glGetUniformLocation(objShaderProgram, "material.diffuse"), cube.material.diffuse.x, cube.material.diffuse.y, cube.material.diffuse.z);
	glUniform3f(glGetUniformLocation(objShaderProgram, "material.specular"), cube.material.specular.x, cube.material.specular.y, cube.material.specular.z);
	glUniform1f(glGetUniformLocation(objShaderProgram, "material.shininess"), cube.material.shininess);
	glUniform3f(glGetUniformLocation(objShaderProgram, "pointLightArr[0].ambient"), pointLight[0].material.ambient.x, pointLight[0].material.ambient.y, pointLight[0].material.ambient.z);
	glUniform3f(glGetUniformLocation(objShaderProgram, "pointLightArr[0].diffuse"), pointLight[0].material.diffuse.x, pointLight[0].material.diffuse.y, pointLight[0].material.diffuse.z);
	glUniform3f(glGetUniformLocation(objShaderProgram, "pointLightArr[0].specular"), pointLight[0].material.specular.x, pointLight[0].material.specular.y, pointLight[0].material.specular.z);
	glUniform1f(glGetUniformLocation(objShaderProgram, "pointLightArr[0].constant"), pointLight[0].attenuation.constant);
	glUniform1f(glGetUniformLocation(objShaderProgram, "pointLightArr[0].linear"), pointLight[0].attenuation.linear);
	glUniform1f(glGetUniformLocation(objShaderProgram, "pointLightArr[0].quadratic"), pointLight[0].attenuation.quadratic);
	glUniform3f(glGetUniformLocation(objShaderProgram, "pointLightArr[0].pos"), pointLight[0].worldSpace.pos.x, pointLight[0].worldSpace.pos.y, pointLight[0].worldSpace.pos.z);
	glUniform3f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].ambient"), spotlight[0].material.ambient.x, spotlight[0].material.ambient.y, spotlight[0].material.ambient.z);
	glUniform3f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].diffuse"), spotlight[0].material.diffuse.x, spotlight[0].material.diffuse.y, spotlight[0].material.diffuse.z);
	glUniform3f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].specular"), spotlight[0].material.specular.x, spotlight[0].material.specular.y, spotlight[0].material.specular.z);
	glUniform1f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].constant"), spotlight[0].attenuation.constant);
	glUniform1f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].linear"), spotlight[0].attenuation.linear);
	glUniform1f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].quadratic"), spotlight[0].attenuation.quadratic);
	glUniform3f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].pos"), spotlight[0].worldSpace.pos.x, spotlight[0].worldSpace.pos.y, spotlight[0].worldSpace.pos.z);
	glUniform3f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].lookDir"), spotlight[0].spotlight.lookDir.x, spotlight[0].spotlight.lookDir.y, spotlight[0].spotlight.lookDir.z);
	glUniform1f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].innerCutoff"), spotlight[0].spotlight.innerCutoff);
	glUniform1f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].outerCutoff"), spotlight[0].spotlight.outerCutoff);
	glUniform1i(glGetUniformLocation(objShaderProgram, "numDirectionalLights"), 0);
	glUniform1i(glGetUniformLocation(objShaderProgram, "numPointLights"), 1);
	glUniform1i(glGetUniformLocation(objShaderProgram, "numSpotlights"), 1);
	glUseProgram(lightShaderProgram);
	glUniform3f(glGetUniformLocation(lightShaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);

	/* set camera and scene positions */
	kmVec3 sceneOrigin, cameraPos;
	kmVec3Fill(&sceneOrigin, 0, 0, 0);
	kmVec3Fill(&cameraPos, 0, 0, 5.0);
	initCamera(&camera, cameraPos, sceneOrigin);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	glfwSwapInterval(0);

	/* main render loop */
	while (!glfwWindowShouldClose(window)) {
		
		char* frameRate = calculateFPS();

		processKeyInput(window);

		/* clear color and depth buffers */
		clearFramebuffer();

		kmVec3Fill(&pointLightPos, 5 * sin(glfwGetTime()), 3, 5 * cos(glfwGetTime()));
		pointLight[0].worldSpace.pos = pointLightPos;

		/* set new flashlight position */
		spotlight[0].worldSpace.pos = camera.cameraPos;
		spotlight[0].spotlight.lookDir = camera.camera_zAxis;

		/* shader program */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *((unsigned int*)get(&cube.material.diffuseMap, 0)));
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, *((unsigned int*)get(&cube.material.diffuseMap, 1)));
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, *((unsigned int*)get(&cube.material.specularMap, 0)));
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, *((unsigned int*)get(&cube.material.emissiveMap, 0)));
		glUseProgram(objShaderProgram);
		glUniform1f(glGetUniformLocation(objShaderProgram, "time"), (float)glfwGetTime());
		glUniform3f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].pos"), camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
		glUniform3f(glGetUniformLocation(objShaderProgram, "spotlightArr[0].lookDir"), -camera.camera_zAxis.x, -camera.camera_zAxis.y, -camera.camera_zAxis.z);
		glUniform1f(glGetUniformLocation(objShaderProgram, "mixAmount"), 0.5);
		glUniform3f(glGetUniformLocation(objShaderProgram, "cameraPos"), camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
		glUniform3f(glGetUniformLocation(objShaderProgram, "pointLightArr[0].pos"), pointLight[0].worldSpace.pos.x, pointLight[0].worldSpace.pos.y, pointLight[0].worldSpace.pos.z);

		/* matrices for cube */
		genDefaultCameraMatrices(&camera);
		kmVec3 scaleFactor;
		kmVec3Fill(&scaleFactor, 2, 2, 2);
		kmVec3 axis;
		kmVec3Fill(&axis, 0, 0, 0);
		genWorldMatrix(&camera, cube.worldSpace.pos, axis, (float)glfwGetTime(), scaleFactor);
		glUniformMatrix4fv(glGetUniformLocation(objShaderProgram, "world"), 1, GL_FALSE, &(camera.worldMatrix.mat[0]));
		glUniformMatrix4fv(glGetUniformLocation(objShaderProgram, "view"), 1, GL_FALSE, &(camera.viewMatrix.mat[0]));
		glUniformMatrix4fv(glGetUniformLocation(objShaderProgram, "projection"), 1, GL_FALSE, &(camera.perspectiveMatrix.mat[0]));
		glEnable(GL_DEPTH_TEST);

		bindAllBuffers(&cubeBuffers);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform1f(glGetUniformLocation(objShaderProgram, "mixAmount"), 1.0);
		glUseProgram(lightShaderProgram);

		/* matrices for light */
		kmVec3Fill(&scaleFactor, 0.25, 0.25, 0.25);
		genWorldMatrix(&camera, pointLight[0].worldSpace.pos, axis, 0.0, scaleFactor);
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram, "world"), 1, GL_FALSE, &(camera.worldMatrix.mat[0]));
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram, "view"), 1, GL_FALSE, &(camera.viewMatrix.mat[0]));
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram, "projection"), 1, GL_FALSE, &(camera.perspectiveMatrix.mat[0]));
		glEnable(GL_DEPTH_TEST);

		bindAllBuffers(&pointLightBuffer1);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSetWindowTitle(window, frameRate);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	endContext();
	return 0;
}
