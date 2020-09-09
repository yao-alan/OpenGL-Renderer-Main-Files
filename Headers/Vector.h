#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct Vector 
{
	char* ptr;
	size_t dataTypeSize;
	size_t currentSize;
	size_t maxSize;
} Vector;

void initVector(Vector* vector, size_t dataTypeSize);

void* get(Vector* vector, int index);

void push_back(Vector* vector, void* item);

void pop_back(Vector* vector);

void resize(Vector* vector);

size_t size(Vector* vector);


