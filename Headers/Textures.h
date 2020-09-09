#pragma once

#include <glad/glad.h>

typedef struct Texture 
{
	unsigned int ID;
	char* type;
} Texture;

unsigned int* useTexture(char* imgFile, GLenum targetBinding);

void configTexMapping(GLenum targetBinding);

void configTexWrapping(GLenum targetBinding);
