#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Buffers.h"

Buffers buffers;

Buffers* genBuffers() 
{
	genVAO();
	genVBO();
	genEBO();

	return &buffers;
}

void bindAllBuffers(Buffers* buffer) 
{
	bindVAO(buffer);
	bindVBO(buffer);
	bindEBO(buffer);
}

void unbindAllBuffers() 
{
	unbindVBO();
	unbindEBO();
	unbindVAO();
}

void genVAO() 
{
	glGenVertexArrays(1, &buffers.VAO);
}

void genVBO() 
{
	glGenBuffers(1, &buffers.VBO);
}

void genEBO() 
{
	glGenBuffers(1, &buffers.EBO);
}

void bindVAO(Buffers* buffer) 
{
	glBindVertexArray((*buffer).VAO);
}

void bindVBO(Buffers* buffer) 
{
	glBindBuffer(GL_ARRAY_BUFFER, (*buffer).VBO);
}

void bindEBO(Buffers* buffer) 
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*buffer).EBO);
}

void unbindVAO() 
{
	glBindVertexArray(0);
}

void unbindVBO() 
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void unbindEBO() 
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void writeToVBO(Buffers* buffer, float* vertexData, unsigned int vertexDataSize, const void* drawType) 
{
	bindAllBuffers(buffer);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, drawType);

}

void writeToEBO(Buffers* buffer, float* vertexData, unsigned int vertexDataSize, const void* drawType) 
{
	bindAllBuffers(buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexDataSize, vertexData, drawType);
}

void clearFramebuffer() 
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
