#pragma once

typedef struct Buffers 
{
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
} Buffers;

Buffers* genBuffers();

void bindAllBuffers(Buffers* buffer);
void unbindAllBuffers();

void genVAO();
void genVBO();
void genEBO();

void bindVAO(Buffers* buffer);
void bindVBO(Buffers* buffer);
void bindEBO(Buffers* buffer);

void unbindVAO();
void unbindVBO();
void unbindEBO();

void writeToVBO(Buffers* buffer, float* vertexData, unsigned int vertexDataSize, const void* drawType);
void writeToEBO(Buffers* buffer, float* vertexData, unsigned int vertexDataSize, const void* drawType);

void clearFramebuffer();
