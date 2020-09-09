#pragma once

typedef struct Shaders 
{
	char* vertexShaderSource;
	char* fragmentShaderSource;
	unsigned int vertexShader;
	unsigned int fragmentShader;
} Shaders;

unsigned int initShaders(char* vertexShaderSource, char* fragmentShaderSource);

void addVertexShaderSource(char* vertexShaderSource);

void addFragmentShaderSource(char* fragmentShaderSource);

void createVertexShader();

void createFragmentShader();

int linkShaders();

void deleteShaders();

void debugShaderCompilation(unsigned int* ptrToShader);